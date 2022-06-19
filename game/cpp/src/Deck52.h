#pragma once


#include "Card52.h"
#include "HoleCards.h"
#include <vector>

namespace game52{

class Deck52{
    public:
        Deck52();
        Deck52(Deck52 const&) = default;
        Deck52& operator=(Deck52 const&) = default;
        Deck52(Deck52&&) = default;
        Deck52& operator=(Deck52&&) = default;

        void shuffle();
        std::string toString() const;
        HoleCards getHoleCards();
        std::vector<Card52> getFlop();
        Card52 getCard();
        std::vector<Card52> getRemainingCards() const;

    void moveToFront(std::vector<Card52> const& cards);
    void shuffleRemainder(); 
    std::pair<std::vector<Card52>::iterator, std::vector<Card52>::iterator> getRange(size_t N);


    private:
        Card52 dealCard();

        std::vector<Card52> cards_;
        int position_ = 0;
};

}