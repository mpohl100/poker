#pragma once

#include "Card52.h"
#include "HoleCards.h"

#include <vector>

namespace game52{

enum Street{
    Preflop,
    Flop,
    Turn,
    River,
};

std::string toString(Street street);

class Board{
    public:
        Board() = default;
        Board(Board const&) = default;
        Board& operator=(Board const&) = default;
        Board(Board&&) = default;
        Board& operator=(Board&&) = default;
        
        Street street() const;
        void dealFlop(std::vector<Card52> const& flop);
        void dealTurn(Card52 turn);
        void dealRiver(Card52 river);
        std::vector<Card52> const& getCards() const;
        std::vector<HoleCards> getBestHoldings() const;
    private:
        std::vector<Card52> cards_;

};

}