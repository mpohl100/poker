#pragma once


#include "Card52.h"
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
    std::string to_string() const;

    private:
        std::vector<Card52> cards_;
};

}