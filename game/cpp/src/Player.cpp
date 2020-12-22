#include "Player.h"
#include "HandHistory.h"

namespace game52{

int Player::get_amount(int amt)
{
    return stack_.get_amount(amt); 
}

void Player::dealHoleCards(HoleCards const& hole_cards)
{
    holeCards_ = hole_cards;
}

bool Player::hasHoleCards() const
{
    return holeCards_.empty();
}

Stack Player::decide(Pot const& pot, Board const& board, HandHistory& handHistory)
{
    HandAction next = engine_.decide(pot, board, handHistory);
    handHistory.logAction(next);
    //if(next.nextBet == 0)
    //    holeCards_ = {};
    return next.nextBet;
}

}