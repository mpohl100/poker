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

        std::string toString() const;
        bool empty() const;
        std::vector<Card52> const& getCards() const;
        static std::vector<HoleCards> getAll();
    private:
        std::vector<Card52> cards_;
};

}