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

class ClassifiedHand{
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

    ClassifiedHand(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end );
    ClassifiedHand()=default;
    ClassifiedHand(ClassifiedHand const&) = default;
    ClassifiedHand& operator=(ClassifiedHand const&) = default;
    ClassifiedHand(ClassifiedHand&&) = default;
    ClassifiedHand& operator=(ClassifiedHand&&) = default;
    
    std::array<Rank52,2> findOccurences(int nb) const;
    int sum() const;
    static ClassifiedHand fromString(std::string const& str);

    std::vector<Card52> cards_;
    HandRank52 handRank_;
    std::map<Rank52, int> rankOccurences_;
};

bool operator<(ClassifiedHand const& l, ClassifiedHand const& r);
bool operator>(ClassifiedHand const& l, ClassifiedHand const& r);
bool operator<=(ClassifiedHand const& l, ClassifiedHand const& r);
bool operator>=(ClassifiedHand const& l, ClassifiedHand const& r);
bool operator==(ClassifiedHand const& l, ClassifiedHand const& r);
bool operator!=(ClassifiedHand const& l, ClassifiedHand const& r);

ClassifiedHand getBestHand(Hand hand);


}