#pragma once

#include "HoleCards.h"
#include "Stack.h"
#include "DecisionEngine.h"

namespace game52{

class Pot;
class Board;
class HandHistory;

class Player{
    public:
        Player(Stack stack, DecisionEngine const& engine);
        Player() = default;
        Player(Player const&) = default;
        Player& operator=(Player const&) = default;
        Player(Player&&) = default;
        Player& operator=(Player&&) = default;

        int get_amount(int amount);
        void dealHoleCards(HoleCards const&);
        bool hasHoleCards() const;
        Stack decide(Pot const& pot, Board const& board, HandHistory& handHistory);

    private:
        Stack stack_;
        DecisionEngine engine_;
        HoleCards holeCards_;
};

}