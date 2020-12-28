#pragma once

#include "Action.h"

#include <optional>

namespace game52{

class Pot;
class Board;
class HandHistory;
class Player;

class DecisionEngine{
public:
    enum class Decision{
        Fold,
        Check,
        Call,
        Raise,
    };
    std::pair<BettingAction, Decision> 
    decide(Pot const& pot, Board const& board, HandHistory& handHistory, Player& hero);
};

}