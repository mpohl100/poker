#include "Hand.h"
#include "HoleCards.h"
#include "Board.h"

namespace game52 {

Hand::Hand(Player* player, HoleCards const& holeCards, Board const& board)
    : player_(player)
{
    cards_ = holeCards.getCards();
    const auto& boardCards = board.getCards();
    cards_.insert(cards_.end(), boardCards.begin(), boardCards.end());
}

int compareHands([[maybe_unused]] Hand left, [[maybe_unused]] Hand right)
{
    return 0;
}

}