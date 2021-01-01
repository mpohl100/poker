#include "Dealer.h"
#include "Action.h"

namespace game52{

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
            getCurrentPot().putAmount(stack.getAmount(allinAmount - prevAllinAmount));
        closedPots_.push_back(getCurrentPot());
        getCurrentPot() = Pot{};
        prevAllinAmount = allinAmount;
    }
    for(const auto& [player, stack] : bets_)
        getCurrentPot().putAmount(stack);
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