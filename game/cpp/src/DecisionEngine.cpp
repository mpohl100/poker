#include "DecisionEngine.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "Player.h"

#include <iostream>

namespace game52{

BettingAction
DecisionEngine::decide( Pot const& pot, 
                        Board const& board, 
                        HandHistory& handHistory,
                        Player& hero)
{    
    auto position = game52::toString(hero.getPosition());
    std::cout << position << '\n';
    std::optional<BettingAction> lastAction = handHistory.getLastBet(board.street());
    std::optional<BettingAction> lastHeroAction = handHistory.getLastBet(board.street(), &hero);
    // hero is opening the action of the street, options are check or raise
    if(not lastAction)
    {
        BettingAction openAction(board.street());
        openAction.player = hero;
        openAction.previousBet = 0;
        switch(rand() % 2){
            case 0: openAction.nextBet = 0; openAction.decision = Decision::Check; break;
            case 1: openAction.nextBet = pot.getAmount() / 2; openAction.decision = Decision::Raise; break;
        }
        return openAction;
    }
    else
    {
        if(not lastHeroAction) // we are acting for the first time in the hand
        {
            BettingAction nextAction(board.street());
            nextAction.player = hero;
            if(lastAction->nextBet == 0) // it has been checked to us
            {
                nextAction.previousBet = 0;
                switch( rand() % 2){
                    case 0: nextAction.nextBet = 0; nextAction.decision = Decision::Check; break;
                    case 1: nextAction.nextBet = pot.getAmount() / 2; nextAction.decision = Decision::Raise; break;
                }
            }
            else
            { // it has been bet to us
                nextAction.toCall = lastAction->nextBet;
                switch( rand() % 3){
                    case 0: nextAction.nextBet = 0; nextAction.decision = Decision::Fold; break;
                    case 1: nextAction.nextBet = lastAction->nextBet; nextAction.decision = Decision::Call; break;
                    case 2: nextAction.nextBet = lastAction->nextBet + lastAction->nextBet; nextAction.decision = Decision::Raise; break; 
                }
            }
            return nextAction;
        }
        else
        {   // we are acting a second time 
            BettingAction nextAction(board.street());
            nextAction.player = hero;
            nextAction.toCall = lastAction->nextBet;
            nextAction.previousBet = lastHeroAction->nextBet;
            if(nextAction.previousBet == nextAction.toCall){
                switch (rand() % 2)
                {
                    case 0: nextAction.nextBet = 0; nextAction.decision = Decision::Check; break;
                    case 1: nextAction.nextBet = pot.getAmount() / 2; nextAction.decision = Decision::Raise; break;
                }
            }
            else
            {
                switch( rand() % 3){
                    case 0: nextAction.nextBet = 0; nextAction.decision = Decision::Fold; break;
                    case 1: nextAction.nextBet = lastAction->nextBet; nextAction.decision = Decision::Call; break;
                    case 2: nextAction.nextBet = lastAction->nextBet + lastAction->nextBet; nextAction.decision = Decision::Raise; break;
                }
            }
            return nextAction;
        }  
    }
}

}