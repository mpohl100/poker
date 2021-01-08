#pragma once

#include "Card52.h"
#include "HoleCards.h"
#include "Board.h"
#include <vector>
#include <map>
#include <array>

namespace game52{

class HoleCards;
class Board;
class Player;

class Hand
{
public:
    Hand(HoleCards const& holeCards, Board const& board);
    Hand() = default;
    Hand(Hand const&) = default;
    Hand& operator=(Hand const&) = default;
    Hand(Hand&&) = default;
    Hand& operator=(Hand&&) = default;
    
    std::vector<Card52> getCards() const;
    HoleCards const& getHoleCards() const;
    std::string toString() const;
private:
    std::vector<Card52> cards_;
    HoleCards holeCards_;
    Board board_;
};

int compareHands(Hand left, Hand right);

class MadeHand52{
public:
    enum HandRank52{
        HighCard,
        Pair,
        TwoPair,
        Trips,
        Straight,
        Flush,
        FullHouse,
        Quads,
        StraightFlush,
    };

    MadeHand52(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end );
    MadeHand52()=default;
    MadeHand52(MadeHand52 const&) = default;
    MadeHand52& operator=(MadeHand52 const&) = default;
    MadeHand52(MadeHand52&&) = default;
    MadeHand52& operator=(MadeHand52&&) = default;
    
    std::array<Rank52,2> findOccurences(int nb) const;
    std::vector<Rank52> getHighCards() const;
    int sum() const;
    static MadeHand52 fromString(std::string const& str);

    std::vector<Card52> cards_;
    HandRank52 handRank_;
    std::map<Rank52, int> rankOccurences_;
};

bool operator<(MadeHand52 const& l, MadeHand52 const& r);
bool operator>(MadeHand52 const& l, MadeHand52 const& r);
bool operator<=(MadeHand52 const& l, MadeHand52 const& r);
bool operator>=(MadeHand52 const& l, MadeHand52 const& r);
bool operator==(MadeHand52 const& l, MadeHand52 const& r);
bool operator!=(MadeHand52 const& l, MadeHand52 const& r);

MadeHand52 getBestHand(Hand hand);

class DrawingHand52{
public:
    enum DrawRank52{
        None,
        WheelGutshot,
        Gutshot,
        Openend,
        Flush,
        FlushWheelGutshot,
        FlushGutshot,
        FlushOpenend,
    };

    DrawingHand52(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end );
    DrawingHand52()=default;
    DrawingHand52(DrawingHand52 const&) = default;
    DrawingHand52& operator=(DrawingHand52 const&) = default;
    DrawingHand52(DrawingHand52&&) = default;
    DrawingHand52& operator=(DrawingHand52&&) = default;

    std::vector<Rank52> getHighCards() const;

    std::vector<Card52> cards_;
    DrawRank52 handRank_;
    std::map<Rank52, int> rankOccurences_;
};

bool operator<(DrawingHand52 const& l, DrawingHand52 const& r);
bool operator>(DrawingHand52 const& l, DrawingHand52 const& r);
bool operator<=(DrawingHand52 const& l, DrawingHand52 const& r);
bool operator>=(DrawingHand52 const& l, DrawingHand52 const& r);
bool operator==(DrawingHand52 const& l, DrawingHand52 const& r);
bool operator!=(DrawingHand52 const& l, DrawingHand52 const& r);


}