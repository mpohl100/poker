#include "Player.h"
#include "HandHistory.h"
#include "Pot.h"

namespace game52{

Player::Player(Stack stack, DecisionEngine const& engine)
    : stack_(stack)
    , engine_(engine)
{}

Stack Player::getAmount(Stack amt)
{
    return stack_.getAmount(amt); 
}

void Player::putAmount(Stack amt)
{
    stack_ += amt; 
}

void Player::dealHoleCards(HoleCards const& hole_cards)
{
    holeCards_ = hole_cards;
}

bool Player::hasHoleCards() const
{
    return holeCards_.empty();
}


HoleCards Player::getHoleCards() const
{
    return holeCards_;
}

Stack Player::decide(Pot& pot, Board const& board, HandHistory& handHistory)
{
    HandAction next = engine_.decide(pot, board, handHistory);
    handHistory.logAction(next);
    if(next.nextBet == 0)
    {
        // fold
        holeCards_ = {};
    }
    else
    {
        // bet,call or raise
        pot.putAmount(next.nextBet);
        getAmount(next.nextBet);
    }
    return next.nextBet;
}

bool Player::ready(Stack amt, Board const& board) const
{
    if(board.street() != lastAction_.street)
        return false;
    return lastAction_.nextBet == amt or not hasHoleCards(); 
}

}