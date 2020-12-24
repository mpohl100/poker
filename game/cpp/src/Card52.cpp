#include "Card52.h" 

#include <stdexcept>
#include <string>

namespace game52{

std::string to_string(Rank52 rank)
{
    switch (rank)
    {
        case Deuce: return "2";
        case Three: return "3";
        case Four: return "4";
        case Five: return "5";
        case Six: return "6";
        case Seven: return "7";
        case Eight: return "8";
        case Nine: return "9";
        case Ten: return "T";
        case Jack: return "J";
        case Queen: return "Q";
        case King: return "K";
        case Ace: return "A";    
    default:
        break;
    }
    return "Unknown";
}

std::string to_string(Suit suit)
{
    switch (suit)
    {
        case Hearts: return "h";
        case Diamonds: return "d";
        case Spades: return "s";
        case Clubs: return "c";
    default:
        break;
    }
    return "Unknown";
}

Rank52 getRank(char c)
{
    switch(c){
        case '2': return Deuce;
        case '3': return Three;
        case '4': return Four;
        case '5': return Five;
        case '6': return Six;
        case '7': return Seven;
        case '8': return Eight;
        case '9': return Nine;
        case 'T': return Ten;
        case 'J': return Jack;
        case 'Q': return Queen;
        case 'K': return King;
        case 'A': return Ace;
        default: throw std::runtime_error("wrong input rank: " + c); 
    }
    return Deuce;
}

Suit getSuit(char c)
{
    switch(c){
        case 'h': return Hearts;
        case 'd': return Diamonds;
        case 's': return Spades;
        case 'c': return Clubs;
        default: throw std::runtime_error("wrong input suit: " + c);
    }
    return Hearts;
}

Card52::Card52(Rank52 rank, Suit suit)
    : nb_(rank*suit)
{}

Card52::Card52(std::string const& card)
    : nb_(int(getRank(card[0]))*int(getSuit(card[1])))
{}

Card52::Card52(int nb)
    : nb_(nb)
{
    if(nb_ < 0 or nb_ > 51)
        throw std::runtime_error("Card52 out of range: " + std::to_string(nb_) );
}
    
Rank52 Card52::rank() const
{
    return Rank52(nb_ / 4);
}

Suit Card52::suit() const
{
    return Suit(nb_ % 4);
}

std::string Card52::to_string() const
{
    std::string s;
    Rank52 r = rank();
    Suit su = suit();
    s += game52::to_string(r);
    s += game52::to_string(su);
    return s;
}

}