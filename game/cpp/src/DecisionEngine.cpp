#include "DecisionEngine.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"

namespace game52{

BettingAction DecisionEngine::decide([[maybe_unused]] Pot const& pot, 
                                    Board const& board, 
                                    HandHistory& handHistory)
{
    // TODO implement
    BettingAction action = handHistory.getLastBet(board.street());
    action.nextBet = 0;
    return action;
}

}