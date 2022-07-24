#pragma once

#include "Card52.h"
#include "HoleCards.h"
#include "Player.h"

#include <map>

namespace game52{

template<class T> class HandRangeCont{
public:
    struct Key{
        Key(Card52 one, Card52 two)
            : suited(one.suit() == two.suit()) 
            , rank1(one.rank())
            , rank2(two.rank())
        {
            if(rank1 < rank2)
                std::swap(rank1, rank2);
        }
        Key(Rank52 one, Rank52 two, bool suited)
            : suited(suited)
            , rank1(one)
            , rank2(two)
        {}
        Key() = default;
        Key(Key const&) = default;
        Key& operator=(Key const&) = default;
        Key(Key&&) = default;
        Key& operator=(Key&&) = default;

        bool suited = false;
        Rank52 rank1;
        Rank52 rank2;
        friend auto operator<=>(Key const& l, Key const& r) = default;
    };
    std::map<Key, T> cont;
};

class HandStrategy{
public:
    HandStrategy(std::string const& str);
    HandStrategy() = default;
    HandStrategy(HandStrategy const&) = default;
    HandStrategy& operator=(HandStrategy const&) = default;
    HandStrategy(HandStrategy&&) = default;
    HandStrategy& operator=(HandStrategy&&) = default;

    std::string toString() const;
private:
    HandRangeCont<std::vector<HoleCards>> hands_;
    HandRangeCont<std::map<HoleCards, Decision>> decisions_;
};

void printPreflopStrengths();

void calculatePreflopStrengths();

}