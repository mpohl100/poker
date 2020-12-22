#pragma once

namespace game52{

enum Rank52{
    Deuce,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace,
};

enum Suit{
    Hearts,
    Diamonds,
    Spades,
    Clubs,
};

class Card52{
    public: 
        Card52(Rank52 rank, Suit suit);
        Card52(int nb);
        Card52() = default;
        Card52(Card52 const&) = default;
        Card52& operator=(Card52 const&) = default;
        Card52(Card52&&) = default;
        Card52& operator=(Card52&&) = default;
    
        Rank52 rank() const;
        Suit suit() const;
        int nb() const;
    private:    
        int nb_ = 0;
};

}