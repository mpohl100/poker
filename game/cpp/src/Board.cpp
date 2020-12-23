#include "Board.h"

namespace game52{

Street Board::street() const
{
    switch(cards_.size()){
        case 0: return Preflop;
        case 3: return Flop;
        case 4: return Turn;
        case 5: return River;
        default: break;
    }
    return Preflop;
}


void Board::dealFlop(std::vector<Card52> const& flop)
{
    cards_ = flop;
}

void Board::dealTurn(Card52 turn)
{
    cards_.push_back(turn);
}

void Board::dealRiver(Card52 river)
{
    cards_.push_back(river);
}


std::vector<Card52> const& Board::getCards() const
{
    return cards_;
}

}