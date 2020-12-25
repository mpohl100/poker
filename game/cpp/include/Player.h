#pragma once

#include "Action.h"
#include "HoleCards.h"
#include "Stack.h"
#include "DecisionEngine.h"

namespace game52{

class Pot;
class Board;
class HandHistory;

enum class Position{
    UTG,
    UTGp1,
    UTGp2,
    Lojack,
    Hijack,
    Cutoff,
    Button,
    SmallBlind,
    BigBlind,
};

std::string toString(Position pos);

class Player{
    public:
        Player(Stack stack, DecisionEngine const& engine);
        Player() = default;
        Player(Player const&) = default;
        Player& operator=(Player const&) = default;
        Player(Player&&) = default;
        Player& operator=(Player&&) = default;

        Stack getAmount(Stack amount);
        void putAmount(Stack amt);
        void dealHoleCards(HoleCards const&);
        bool hasHoleCards() const;
        HoleCards getHoleCards() const;
        Stack decide(Pot& pot, Board const& board, HandHistory& handHistory);
        bool ready(Stack amt, Board const& board) const;
        void setPlayerIndex(int index, size_t size);
        Position getPosition() const;
    private:
        HandAction lastAction_;
        Stack stack_;
        DecisionEngine engine_;
        HoleCards holeCards_;
        std::pair<int, size_t> pos_; // to deduce the position;
};

}