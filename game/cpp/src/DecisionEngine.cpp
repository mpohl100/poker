#include "DecisionEngine.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "Player.h"

namespace game52{

std::pair<BettingAction, DecisionEngine::Decision>
DecisionEngine::decide( Pot const& pot, 
                        Board const& board, 
                        HandHistory& handHistory,
                        Player& hero)
{    
    std::optional<BettingAction> lastAction = handHistory.getLastBet(board.street());
    std::optional<BettingAction> lastHeroAction = handHistory.getLastBet(board.street(), &hero);
    DecisionEngine::Decision decision;
    // hero is opening the action of the street, options are check or raise
    if(not lastAction)
    {
        BettingAction openAction(board.street());
        openAction.player.reset(&hero);
        openAction.previousBet = 0;
        switch(rand() % 2){
            case 0: openAction.nextBet = 0; decision = Decision::Check; break;
            case 1: openAction.nextBet = pot.getAmount() / 2; decision = Decision::Raise; break;
        }
        return {openAction, decision};
    }
    else
    {
        if(not lastHeroAction) // we are acting for the first time in the hand
        {
            BettingAction nextAction(board.street());
            nextAction.player.reset(&hero);
            if(lastAction->nextBet == 0) // it has been checked to us
            {
                nextAction.previousBet = 0;
                switch( rand() % 2){
                    case 0: nextAction.nextBet = 0; decision = Decision::Check; break;
                    case 1: nextAction.nextBet = pot.getAmount() / 2; decision = Decision::Raise; break;
                }
            }
            else
            { // it has been bet to us
                nextAction.previousBet = lastAction->nextBet;
                switch( rand() % 3){
                    case 0: nextAction.nextBet = 0; decision = Decision::Fold; break;
                    case 1: nextAction.nextBet = lastAction->nextBet; decision = Decision::Call; break;
                    case 2: nextAction.nextBet = lastAction->nextBet + lastAction->nextBet; decision = Decision::Raise; break; 
                }
            }
            return {nextAction, decision};
        }
        else
        {   // we are acting a second time 
            BettingAction nextAction(board.street());
            nextAction.player.reset(&hero);
            nextAction.previousBet = lastHeroAction->nextBet;
            switch( rand() % 3){
                case 0: nextAction.nextBet = 0; decision = Decision::Fold; break;
                case 1: nextAction.nextBet = lastAction->nextBet; decision = Decision::Call; break;
                case 2: nextAction.nextBet = lastAction->nextBet + lastAction->nextBet; decision = Decision::Raise; break;
            }
            return {nextAction, decision};
        }  
    }
}

}