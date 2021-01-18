#include "RatedHand.h"
#include "Hand.h"

#include <cmath>

namespace game52{

RatedHand::RatedHand(HoleCards const& holeCards, Board const& board, Deck52 const& deck)
    : holeCards_(holeCards)
    , board_(board)
    , deck_(deck)
{}


float RatedHand::getScore() const
{
    float madeHandScore = getMadeHandScore();
    float drawingHandScore = getDrawingHandScore();
    return madeHandScore + drawingHandScore;
}

float RatedHand::getMadeHandScore() const
{
    MadeHand52 madeHand = getBestHand(Hand(holeCards_, board_));
    float baseScore = 52*51*int(madeHand.handRank_);

     return baseScore;
}

float RatedHand::getDrawingHandScore() const
{
    return 0;
}

}