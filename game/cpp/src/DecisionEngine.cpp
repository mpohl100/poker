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
    std::optional<BettingAction> action = handHistory.getLastBet(board.street());
    if(action)
    {
        action->nextBet = 0;
        return *action;
    }
    return BettingAction(board.street());
}

}