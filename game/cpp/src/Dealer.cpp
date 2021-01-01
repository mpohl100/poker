#include "Dealer.h"
#include "Action.h"

#include<range/v3/all.hpp>

namespace game52{

Dealer::Dealer(Stack bigBlind)
    : bigBlind_(bigBlind)
{}

Options Dealer::getOptions(Player& player)
{
    if(not player.hasHoleCards() or player.isAllin())
        return {};
    auto madeBets = bets_ | ranges::view::values  | ranges::view::unique | ranges::to<std::vector> 
                          | ranges::action::sort(std::greater<Stack>());
    auto currentBetR = madeBets | ranges::view::take(1);
    auto previousBetR = madeBets | ranges::view::drop(1) | ranges::view::take(1);
    Stack currentBet = 0;
    if(ranges::distance(currentBetR) == 1)
        currentBet = *ranges::begin(currentBetR);
    Stack previousBet = 0;
    if(ranges::distance(previousBetR) == 1)
        previousBet = *ranges::begin(previousBetR);
    Options ret;
    if(currentBet == Stack(0))
        ret.options = { {Decision::Check, Stack(0)}, 
                        {Decision::Raise, Stack(bigBlind_)} };
    else
        ret.options = { {Decision::Fold, Stack(0)}, 
                        {Decision::Call, Stack(currentBet)}, 
                        {Decision::Raise, currentBet - previousBet} };
    return ret;
}

void Dealer::acceptBet(Player& player, BettingAction const& bettingAction)
{
    Stack toBeBooked = bettingAction.nextBet - bettingAction.previousBet;
    Stack receivedBet = player.getAmount(toBeBooked);
    bets_[&player] += receivedBet;
}

void Dealer::rakeIn()
{
    std::map<Stack,int> allinAmounts;
    for(const auto& [player, stack] : bets_)
        if(player->isAllin())
            allinAmounts[stack]++;
    Stack prevAllinAmount = Stack(0);
    for(const auto& [allinAmount, nb] : allinAmounts)
    {
        for(auto& [player, stack] : bets_)
            getCurrentPot().putAmount(player, stack.getAmount(allinAmount - prevAllinAmount));
        closedPots_.push_back(getCurrentPot());
        getCurrentPot() = Pot{};
        prevAllinAmount = allinAmount;
    }
    for(const auto& [player, stack] : bets_)
        getCurrentPot().putAmount(player, stack);
    bets_ = {};
}

Pot& Dealer::getCurrentPot()
{
    return currentPot_;
}

void Dealer::showdown() const
{

}

}