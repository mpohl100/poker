#pragma once

#include "Card52.h"

#include <vector>

namespace game52{

enum Street{
    Preflop,
    Flop,
    Turn,
    River,
};

class Board{
    public:
        Street street() const;
        void dealFlop(std::vector<Card52> const& flop);
        void dealTurn(Card52 turn);
        void dealRiver(Card52 river);
    private:
        std::vector<Card52> cards_;

};

}