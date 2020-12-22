#pragma once

#include "Action.h"

namespace game52{

class Pot;
class Board;
class HandHistory;

class DecisionEngine{
    public:
        HandAction decide(Pot const& pot, Board const& board, HandHistory& handHistory);
};

}