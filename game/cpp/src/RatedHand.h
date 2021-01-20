#pragma once

#include "HoleCards.h"
#include "Board.h"
#include "Deck52.h"


namespace game52{

class RatedHand{
public:
    RatedHand(HoleCards const& holeCards, Board const& board, Deck52 const& deck);
    RatedHand() = default;
    RatedHand(RatedHand const&)  = default;
    RatedHand& operator=(RatedHand const&) = default;
    RatedHand(RatedHand&&)  = default;
    RatedHand& operator=(RatedHand&&) = default;

    float getScore() const;

private:
    float getMadeHandScore() const;
    float getDrawingHandScore() const;
    HoleCards holeCards_;
    Board board_;
    Deck52 deck_;
};

}