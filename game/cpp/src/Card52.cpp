#include "Card52.h" 

#include <stdexcept>
#include <string>

namespace game52{

Card52::Card52(Rank52 rank, Suit suit)
    : nb_(rank*suit)
{}

Card52::Card52(int nb)
    : nb_(nb)
{
    if(nb_ < 0 or nb_ > 51)
        throw std::runtime_error("Card52 out of range: " + std::to_string(nb_) );
}
    
Rank52 Card52::rank() const
{
    return Rank52(nb_ % 4);
}

Suit Card52::suit() const
{
    return Suit(nb_ / 4);
}

}