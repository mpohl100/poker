#include "Action.h"
#include "Player.h"
#include "Stack.h"

namespace game52 {

std::string HandAction::toString() const
{
    std::string ret = game52::toString(player->getPosition()); 
    if( nextBet == 0)
        ret += " folds.";
    else if( nextBet == previousBet )
        ret += " calls " + nextBet.toString() + ".";
    else
        ret += " raises " + (nextBet - previousBet).toString() + " to " + nextBet.toString() + ".";
    return ret;
}

}