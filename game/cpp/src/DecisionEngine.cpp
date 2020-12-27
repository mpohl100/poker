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
        action->previousBet = action->nextBet;
        switch( rand() % 3){
            case 0: action->nextBet = 0; break;// check or fold
            case 1: action->nextBet = action->previousBet; break;
            case 2: action->nextBet = action->previousBet + action->previousBet; break;
        }
        return *action;
    }
    return BettingAction(board.street());
}

}