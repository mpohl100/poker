#include "Deck52.h"

#include <algorithm>
#include <random>

namespace game52{

Deck52::Deck52()
{
    for(int i = 0; i < 52; ++i)
        cards_.push_back(Card52(i));
}

void Deck52::shuffle()
{
    static std::random_device rng{};
    static std::mt19937 generator(rng());
    std::shuffle(std::begin(cards_), std::end(cards_), generator);
}

std::string Deck52::to_string() const
{
    std::string s;
    for( const auto& card : cards_)
        s += card.to_string() + " ";
    return s;
}

}