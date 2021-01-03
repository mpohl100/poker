#pragma once

#include "Pot.h"
#include "Player.h"

#include <map>

namespace game52{

class BettingAction;

struct Options{
    std::vector<std::pair<Decision, std::pair<Stack,Stack>>> options;
};

class Dealer{
public:
    Dealer(std::vector<std::reference_wrapper<Player>> const& players, Stack bigBlind);
    Dealer() = delete;
    Dealer(Dealer const&) = default;
    Dealer& operator=(Dealer const&) = default;
    Dealer(Dealer&&) = default;
    Dealer& operator=(Dealer&&) = default;
    Options getOptions(Player& player) const;
    void acceptBet(Player& player, BettingAction const& bettingAction);
    void rakeIn();
    Pot& getCurrentPot();
    void showdown() const;
    Stack getCurrentBet(Player const& player) const;
protected:
    std::vector<std::reference_wrapper<Player>> players_;
    Stack bigBlind_;
    mutable std::unique_ptr<std::optional<Stack>> maxAmount_;
    std::map<const Player*, Stack> bets_;
    std::vector<Pot> closedPots_;
    Pot currentPot_;
};

}