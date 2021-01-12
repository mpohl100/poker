#include "Dealer.h"
#include "Action.h"
#include "HandHistory.h"

#include <range/v3/all.hpp>
#include <algorithm>

namespace game52{

Dealer::Dealer(std::vector<std::reference_wrapper<Player>> const& players, Stack bigBlind)
    : players_(players) 
    , bigBlind_(bigBlind)
{}

auto getMadeBets(std::map<const Player*,Stack> bets)
{
    return bets | ranges::views::values | ranges::to<std::vector<Stack>> | ranges::actions::unique
                | ranges::actions::sort(std::greater<Stack>{});
}

std::pair<Stack, Stack> getCurrentBets(std::map<const Player*, Stack> const& bets)
{
    auto madeBets = getMadeBets(bets);
    auto currentBetR = madeBets | ranges::views::take(1);
    auto previousBetR = madeBets | ranges::views::drop(1) | ranges::views::take(1);
    Stack currentBet = 0;
    if(ranges::distance(currentBetR) == 1)
        currentBet = *ranges::begin(currentBetR);
    Stack previousBet = 0;
    if(ranges::distance(previousBetR) == 1)
        previousBet = *ranges::begin(previousBetR);
    return {currentBet, previousBet};
}

Stack getAllinAmount(Player& player, std::map<const Player*, Stack> const& bets)
{
    if(bets.find(&player) == bets.end())
        return Stack(0);
    return bets.at(&player);
}

auto getLeftInHand(std::vector<std::reference_wrapper<Player>> const& players)
{
    return players | ranges::views::filter([](const auto& p){ return p.get().hasHoleCards();});
}

bool Dealer::actionRequired(Player& player) const
{
    if(bets_.find(&player) == bets_.end()) // player has not bet yet
        return true;
    auto madeBets = getMadeBets(bets_);
    auto highestBetR = madeBets | ranges::views::take(1);
    Stack highestBet = 0;
    if(ranges::distance(highestBetR) == 1)
        highestBet = *ranges::begin(highestBetR);
    if(street_ == Preflop and &player == &bigBlindPlayer() and highestBet == bigBlind_)
    {
        int nbPlayersWithHand = ranges::count_if(players_, [](const auto& p){ return p.get().hasHoleCards();});
        return nbPlayersWithHand > 1;
    }
    return highestBet != bets_.at(&player);
}

Stack getMaxAmount(std::vector<std::reference_wrapper<Player>> const& players)
{
    auto leftInHand = getLeftInHand(players);
    auto secondBiggestAmountR = leftInHand  | ranges::views::transform([](const auto& p){ return p.get().getStack();})
                                            | ranges::to<std::vector>
                                            | ranges::actions::sort(std::greater<Stack>{});
    if(ranges::distance(secondBiggestAmountR) > 1)
        return *ranges::next(ranges::begin(secondBiggestAmountR));
    throw std::runtime_error("max amount could not be deduced as not enough players are left in the hand");
}

Stack takeSmaller(Stack l, Stack r)
{
    return l < r ? l : r;
}

Stack takeBigger(Stack l, Stack r)
{
    return l > r ? l : r;
}

const Player& Dealer::bigBlindPlayer() const
{
    if(players_.size() == 2)
        return players_[1].get();
    return players_[2].get();
}

Options Dealer::getOptions(Player& player) const
{
    if(not player.hasHoleCards() or player.isAllin(getAllinAmount(player, bets_)))
        return {};
    if(not actionRequired(player))
        return {};
    auto [currentBet, previousBet] = getCurrentBets(bets_);
    Options ret;
    // only calculate maxAmount once per betting round
    Stack maxAmount = getMaxAmount(players_);
    maxAmount = takeSmaller(maxAmount, player.getStack());
    if(maxAmount == Stack(0))
        return {};
    if(currentBet == Stack(0) or (street_ == Preflop and currentBet == Stack(bigBlind_) and &player == &bigBlindPlayer() ))
        ret.options = { {Decision::Check, {Stack(0), Stack(0)}}, 
                        {Decision::Raise, {takeSmaller(bigBlind_, maxAmount), maxAmount}} };
    else if(currentBet < maxAmount)
        ret.options = { {Decision::Fold, {Stack(0),Stack(0)}}, 
                        {Decision::Call, {currentBet, currentBet} }, 
                        {Decision::Raise, {takeSmaller(currentBet + takeBigger(currentBet - previousBet, bigBlind_), maxAmount), maxAmount}} };
    else
        ret.options =  { {Decision::Fold, {Stack(0), Stack(0)} },
                         {Decision::Call, {maxAmount, maxAmount} } };
    return ret;
}

void Dealer::acceptBet(Player& player, BettingAction const& bettingAction)
{
    Stack toBeBooked = bettingAction.nextBet - bets_[&player];
    if(toBeBooked < 0)
        toBeBooked = Stack(0);
    bets_[&player] += toBeBooked;
    street_ = bettingAction.street;
}

bool allinIsCalled(Stack allin, std::map<const Player*, Stack> const& bets)
{
    auto sortedBets = bets | ranges::views::transform([](const auto& pr){ return pr.second; })
                           | ranges::to<std::vector>
                           | ranges::actions::sort
                           | ranges::actions::reverse;
    auto secondLargestIt = ranges::next(ranges::begin(sortedBets));
    if(secondLargestIt != ranges::end(sortedBets))
        return *secondLargestIt >= allin;
    return false; 
}

void Dealer::rakeIn()
{
    std::map<Stack,int> allinAmounts;
    for(const auto& [player, stack] : bets_)
        if(player->isAllin(stack) and allinIsCalled(stack, bets_))
            allinAmounts[stack]++;
    Stack prevAllinAmount = Stack(0);
    for(const auto& [allinAmount, nb] : allinAmounts)
    {
        for(auto& [player, stack] : bets_)
            getCurrentPot().putAmount(player, player->getAmount(stack.getAmount(allinAmount - prevAllinAmount)));
        closedPots_.push_back(getCurrentPot());
        getCurrentPot() = Pot{};
        prevAllinAmount = allinAmount;
    }
    for(const auto& [player, stack] : bets_)
        getCurrentPot().putAmount(player, player->getAmount(stack));
    bets_ = {};
}

std::vector<Stack> dividePot(Stack pot, int N)
{
    if(pot == Stack(0))
        return {Stack(0)};
    if(N == 0)
        throw std::runtime_error("trying to divide pot " + std::to_string(pot.amount_) + " to " + std::to_string(N) + " players.");
    // damit bei Split Pot keine chips verloren gehen
    std::vector<Stack> ret(N, pot / N);
    int rest = pot.amount_ % N;
    for(int i = 0; i < rest; ++i)
        ret[i] += Stack(1);
    assert(pot == std::accumulate(ret.begin(), ret.end(), Stack(0)));
    return ret;
}

void showdown(Pot& pot, Board const& board, HandHistory& handHistory)
{
    // show down
    std::optional<std::pair<Hand, std::vector<const Player*>>> currentWinner;
    auto players = pot.getBets() | ranges::views::transform([](const auto& pr){ return pr.first; })
                                 | ranges::to<std::vector>;
    for(auto& player : players | ranges::views::filter([](const auto& p){ return p->hasHoleCards();}))
    {
        if( not currentWinner)
        {
            Hand firstHand = Hand(player->getHoleCards(), board);
            currentWinner = std::pair{ firstHand, std::vector{player} };
            handHistory.logAction(std::make_unique<ShowdownAction>(*player, firstHand));
        }
        else
        {
            Hand newHand(player->getHoleCards(), board);
            int result = compareHands(newHand, currentWinner->first);
            switch(result){
                case 1: 
                {
                    currentWinner = std::pair{newHand, std::vector{player}};
                    handHistory.logAction(std::make_unique<ShowdownAction>(*player, newHand));
                } break;
                case 0:
                {
                    currentWinner->second.push_back(player);
                    handHistory.logAction(std::make_unique<ShowdownAction>(*player, newHand));
                }
                break;
                case -1: handHistory.logAction(std::make_unique<ShowdownAction>(*player, std::nullopt)); break; // do nothing 
            }
        }
    }
    std::vector<Stack> amounts = dividePot(pot.getAmount(), int(currentWinner->second.size()));
    int i = 0;
    for(const auto& player : currentWinner->second)
    {
        Stack amount = amounts[i++];
        player->putAmount(amount);
        handHistory.logAction(std::make_unique<PotAction>(*player, amount, pot));
    }
}

bool Dealer::awardPots(Board const& board, HandHistory& handHistory)
{
    if(closedPots_.empty())
    {
        int playersWithHand = ranges::count_if(getCurrentPot().getBets(), [](const auto& pr){ return pr.first->hasHoleCards();});
        if(playersWithHand == 1)
        {
            auto it = std::find_if( getCurrentPot().getBets().begin(),  getCurrentPot().getBets().end(),
                                    [](const auto& p){return p.first->hasHoleCards();} );
            Stack amount = getCurrentPot().get();
            it->first->putAmount(amount);
            handHistory.logAction(std::make_unique<PotAction>(*it->first, amount, getCurrentPot()));
            return true; // finished
        } 
    }
    if(board.street() == River)
    {
        showdown(getCurrentPot(), board, handHistory);
        for(auto& sidepot : closedPots_ | ranges::views::reverse)
            showdown(sidepot, board, handHistory);
        return true;
    }
    return false;
}

Pot& Dealer::getCurrentPot()
{
    return currentPot_;
}

Stack Dealer::getCurrentBet(Player const& player) const
{
    if(bets_.find(&player) == bets_.end())
        return Stack(0);
    return bets_.at(&player);
}

}