#pragma once

#include "Pot.h"
#include "Player.h"

#include <map>

namespace game52{

class BettingAction;

class Dealer{
public:
    Dealer() = default;
    Dealer(Dealer const&) = default;
    Dealer& operator=(Dealer const&) = default;
    Dealer(Dealer&&) = default;
    Dealer& operator=(Dealer&&) = default;
    void acceptBet(Player& player, BettingAction const& bettingAction);
    void rakeIn();
    Pot& getCurrentPot();
    void showdown() const;
protected:
    std::map<Player*, Stack> bets_;
    std::vector<Pot> closedPots_;
    Pot currentPot_;
};

}