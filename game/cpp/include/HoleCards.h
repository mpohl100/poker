#pragma once

#include "Card52.h"
#include <vector>

namespace game52{

class HoleCards
{
    public:
        HoleCards(Card52 first, Card52 second);
        HoleCards() = default;
        HoleCards(HoleCards const&) = default;
        HoleCards& operator=(HoleCards const&) = default;
        HoleCards(HoleCards&&) = default;
        HoleCards& operator=(HoleCards&&) = default;

        bool empty() const;
    private:
        std::vector<Card52> cards_;
};

}