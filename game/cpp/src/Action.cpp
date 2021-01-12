#include "Action.h"
#include "Player.h"
#include "Stack.h"

#include <range/v3/all.hpp>


namespace game52 {

HandAction::HandAction(Player const& player)
    : player(player)
{}


BettingAction::BettingAction(Player const& player, Street street)  
    : HandAction(player) 
    , decision(Decision::Check)
    , street(street)
{}

std::string BettingAction::toString() const
{
    std::string ret = player.getName() + ": "; 
    if( decision == Decision::Fold)
        ret += " folds.";
    else if( decision == Decision::Check )
        ret += " checks.";
    else if( decision == Decision::Call )
        ret += " calls " + (toCall-previousBet).toString() + "."; 
    else
        ret += " raises " + (nextBet - toCall).toString() + " to " + nextBet.toString() + ".";
    return ret;
}

BettingAction BettingAction::create(  Player const& player, Street street, 
                    Stack nextBet, Stack previousBet, 
                    Stack toCall, Decision decision)
{
    BettingAction bet(player, street);
    bet.nextBet = nextBet;
    bet.previousBet = previousBet;
    bet.toCall = toCall;
    bet.decision = decision;
    return bet;
}

std::string DealingAction::toString() const
{
    return "dealt to " + player.getName() + " [" + holeCards.toString() + "]";
}

std::string BoardAction::toString() const
{
    std::string s = game52::toString(board.street());
    std::string firstCards;
    int N = board.street() == Flop  ? int(board.getCards().size()) 
                                    : int(board.getCards().size()) - 1;
    for(int i = 0; i < N; ++i)
        firstCards += board.getCards()[i].toString() + " ";
    if(board.street() >= Flop)
        firstCards = firstCards | ranges::views::take(ranges::size(firstCards)-1) | ranges::to<std::string>();
    if(board.street() > Preflop)
        firstCards = '[' + firstCards + ']';
    if(board.street() >= Turn)
        firstCards += " ["+ board.getCards().back().toString() + "]";
    if(not firstCards.empty())
        s += "\n" + firstCards;
    return s; 
}

std::string SeatingAction::toString() const
{
    return "Seat " + std::to_string(player.getNumber()) + ": " 
        + player.getName() + "(" + game52::toString(player.getPosition()) + ")" 
        + " (" + player.getStack().toString() + " in chips)"  ;
}

ShowdownAction::ShowdownAction(Player const& player, std::optional<Hand> const& hand)
    : HandAction(player)
    , hand(hand)
{}

std::string ShowdownAction::toString() const
{
    if(not hand)
        return player.getName() + " mucks.";
    return player.getName() + " shows [" + hand->getHoleCards().toString() + "], " + hand->toString();
}

PotAction::PotAction(Player const& player, Stack amount, Pot const& pot)
    : HandAction(player)
    , amount(amount)
    , pot(pot)
{}

std::string PotAction::toString() const
{
    return player.getName() + " wins " + amount.toString() + " from " + pot.toString(); 
}

std::string SummaryAction::toString() const
{
    return "";
}

}