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
    position_ = 0;
}

std::string Deck52::to_string() const
{
    std::string s;
    for( const auto& card : cards_)
        s += card.to_string() + " ";
    return s;
}

HoleCards Deck52::getHoleCards()
{
    Card52 card1 = dealCard();
    Card52 card2 = dealCard();
    return HoleCards(card1, card2);
}

Card52 Deck52::dealCard()
{
    Card52 card = cards_[position_];
    position_ += 1;
    return card;
}

std::vector<Card52> Deck52::getFlop()
{
    std::vector<Card52> flop;
    // burn card
    dealCard();
    for(int i = 0; i < 3; ++i)
        flop.push_back(dealCard());
    return flop;
}

Card52 Deck52::getCard()
{
    // burn card
    dealCard();
    return dealCard();
}

}