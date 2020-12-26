#include "Action.h"
#include "Player.h"
#include "Stack.h"

namespace game52 {

BettingAction::BettingAction(Street street) 
    : street(street) 
{}

std::string BettingAction::toString() const
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

std::string DealingAction::toString() const
{
    return "";
}

std::string BoardAction::toString() const
{
    return "";
}

std::string SeatingAction::toString() const
{
    return "";
}

std::string ShowdownAction::toString() const
{
    return "";
}

std::string SummaryAction::toString() const
{
    return "";
}

}