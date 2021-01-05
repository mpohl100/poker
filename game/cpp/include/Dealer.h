#pragma once

#include "Pot.h"
#include "Player.h"
#include "Board.h"

#include <map>

namespace game52{

class BettingAction;
class HandHistory;

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
    bool awardPots(Board const& board, HandHistory& handHistory);
    Pot& getCurrentPot();
    Stack getCurrentBet(Player const& player) const;
    const Player& bigBlindPlayer() const;
protected:
    bool actionRequired(Player& player) const;
    std::vector<std::reference_wrapper<Player>> players_;
    Stack bigBlind_;
    Street street_ = Preflop;
    std::map<const Player*, Stack> bets_;
    std::vector<Pot> closedPots_;
    Pot currentPot_;
};

}