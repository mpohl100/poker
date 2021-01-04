#include "Dealer.h"
#include "Action.h"

#include <range/v3/all.hpp>
#include <algorithm>

namespace game52{

Dealer::Dealer(std::vector<std::reference_wrapper<Player>> const& players, Stack bigBlind)
    : players_(players) 
    , bigBlind_(bigBlind)
{}

auto getMadeBets(std::map<const Player*,Stack> bets)
{
    return bets | ranges::view::values | ranges::to<std::vector<Stack>> | ranges::action::unique
                | ranges::action::sort(std::greater<Stack>{});
}

std::pair<Stack, Stack> getCurrentBets(std::map<const Player*, Stack> const& bets)
{
    auto madeBets = getMadeBets(bets);
    auto currentBetR = madeBets | ranges::view::take(1);
    auto previousBetR = madeBets | ranges::view::drop(1) | ranges::view::take(1);
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
    return players | ranges::view::filter([](const auto& p){ return p.get().hasHoleCards();});
}

bool Dealer::actionRequired(Player& player) const
{
    if(bets_.find(&player) == bets_.end()) // player has not bet yet
        return true;
    auto madeBets = getMadeBets(bets_);
    auto highestBetR = madeBets | ranges::view::take(1);
    Stack highestBet = 0;
    if(ranges::distance(highestBetR) == 1)
        highestBet = *ranges::begin(highestBetR);
    if(street_ == Preflop and &player == &bigBlindPlayer() and highestBet == bigBlind_)
        return true;
    return highestBet != bets_.at(&player);
}

Stack getMaxAmount(std::vector<std::reference_wrapper<Player>> const& players)
{
    auto leftInHand = getLeftInHand(players);
    auto secondBiggestAmountR = leftInHand  | ranges::view::transform([](const auto& p){ return p.get().getStack();})
                                            | ranges::to<std::vector>
                                            | ranges::action::sort(std::greater<Stack>{});
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
    Stack toBeBooked = bettingAction.nextBet - bettingAction.previousBet;
    bets_[&player] += toBeBooked;
    street_ = bettingAction.street;
}

bool allinIsCalled(Stack allin, std::map<const Player*, Stack> const& bets)
{
    auto sortedBets = bets | ranges::view::transform([](const auto& pr){ return pr.second; })
                           | ranges::to<std::vector>
                           | ranges::action::sort
                           | ranges::action::reverse;
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

Pot& Dealer::getCurrentPot()
{
    return currentPot_;
}

void Dealer::showdown() const
{

}

Stack Dealer::getCurrentBet(Player const& player) const
{
    if(bets_.find(&player) == bets_.end())
        return Stack(0);
    return bets_.at(&player);
}

}