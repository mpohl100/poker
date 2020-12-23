#include "HoleCards.h"

namespace game52{

HoleCards::HoleCards(Card52 first, Card52 second)
    : cards_({first, second})
{}

bool HoleCards::empty() const
{
    return cards_.empty();
}

std::vector<Card52> const& HoleCards::getCards() const
{
    return cards_;
}

}