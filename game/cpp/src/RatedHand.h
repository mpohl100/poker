#pragma once

#include "HoleCards.h"
#include "Board.h"
#include "Deck52.h"


namespace game52{

class RatedHand{
public:
    RatedHand(HoleCards const& holeCards, Board const& board);
    RatedHand() = default;
    RatedHand(RatedHand const&)  = default;
    RatedHand& operator=(RatedHand const&) = default;
    RatedHand(RatedHand&&)  = default;
    RatedHand& operator=(RatedHand&&) = default;

    float getScore() const;
    double getSimulatedScore() const;

private:

    std::vector<Board> getBoards() const;
    float getMadeHandScore() const;
    float getDrawingHandScore() const;
    HoleCards holeCards_;
    Board board_;
    size_t nbPreflopBoards_ = 1000;
};

}