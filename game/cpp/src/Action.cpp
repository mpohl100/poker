#include "Action.h"
#include "Player.h"
#include "Stack.h"

#include <range/v3/all.hpp>


namespace game52 {

BettingAction::BettingAction(Street street) 
    : street(street) 
{}

std::string BettingAction::toString() const
{
    std::string ret = game52::toString(player.getPosition()); 
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
    return "dealt to " + game52::toString(player.getPosition()) + " [" + holeCards.toString() + "]";
}

std::string BoardAction::toString() const
{
    std::string s = game52::toString(board.street()) + '\n';
    std::string firstCards;
    int N = board.street() == Flop  ? int(board.getCards().size()) 
                                    : int(board.getCards().size()) - 1;
    for(int i = 0; i < N; ++i)
        firstCards += board.getCards()[i].toString() + " ";
    if(board.street() >= Flop)
        firstCards = firstCards | ranges::view::take(ranges::size(firstCards)-1) | ranges::to<std::string>();
    if(board.street() > Preflop)
        firstCards = '[' + firstCards + ']';
    if(board.street() >= Turn)
        firstCards += " ["+ board.getCards().back().toString() + "]";
    return s + firstCards; 
}

std::string SeatingAction::toString() const
{
    return "Seat " + std::to_string(player.getNumber()) + ": " 
        + game52::toString(player.getPosition()) 
        + " (" + startingStack.toString() + " in chips)"  ;
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