#include "DecisionEngine.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"

namespace game52{

HandAction DecisionEngine::decide([[maybe_unused]] Pot const& pot, [[maybe_unused]] Board const& board, HandHistory& handHistory)
{
    // TODO implement
    HandAction action = handHistory.getLast();
    action.nextBet = 0;
    return action;
}

}