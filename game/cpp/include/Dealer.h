#pragma once

#include "Pot.h"
#include "Player.h"

#include <map>

namespace game52{

class BettingAction;

struct Options{
    std::vector<std::pair<Decision, Stack>> options;
};

class Dealer{
public:
    Dealer(Stack bigBlind);
    Dealer() = delete;
    Dealer(Dealer const&) = default;
    Dealer& operator=(Dealer const&) = default;
    Dealer(Dealer&&) = default;
    Dealer& operator=(Dealer&&) = default;
    Options getOptions(Player& player);
    void acceptBet(Player& player, BettingAction const& bettingAction);
    void rakeIn();
    Pot& getCurrentPot();
    void showdown() const;
protected:
    Stack bigBlind_;
    std::map<Player*, Stack> bets_;
    std::vector<Pot> closedPots_;
    Pot currentPot_;
};

}