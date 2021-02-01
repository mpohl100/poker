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
    Rank52 relevantCard = madeHand.getRelevantCard();
    Rank52 kicker = madeHand.getKicker();
    float baseScore = 13*13*int(madeHand.handRank_);
    return baseScore + 13*int(relevantCard) + int(kicker);
}

float RatedHand::getDrawingHandScore() const
{
    AllDraws allDraws(Hand(holeCards_, board_));
    float topPair = int(Ten)*13 + int(Jack) + 13*13*int(MadeHand52::Pair);
    int outs = allDraws.getOuts();
    topPair *= outs / 9.0;
    return int(topPair);
}

}