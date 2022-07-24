#include "HoleCards.h"

namespace game52{

HoleCards::HoleCards(Card52 first, Card52 second)
    : cards_({first, second})
{}

std::string HoleCards::toString() const
{
    return cards_[0].toString() + " " + cards_[1].toString();
}

bool HoleCards::empty() const
{
    return cards_.empty();
}

std::vector<Card52> const& HoleCards::getCards() const
{
    return cards_;
}

std::vector<HoleCards> HoleCards::getAll()
{
    std::vector<HoleCards> ret;
    auto allCards = Card52::getAll();
    for(Card52 const& card1 : allCards)
        for(Card52 const& card2 : allCards)
            if(card1 != card2)
                ret.push_back(HoleCards(card1, card2));
    return ret;
}

}