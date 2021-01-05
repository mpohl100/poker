#pragma once

#include "HoleCards.h"
#include "Stack.h"

#include <memory>

namespace game52{

class Pot;
class Board;
class HandHistory;
class DecisionEngine;
class Dealer;

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

enum class Decision{
    Fold,
    Check,
    Call,
    Raise,
};

std::string toString(Position pos);

class Player{
    public:
        Player(Stack stack, DecisionEngine const& engine, int nb);
        Player() = default;
        Player(Player const&) = default;
        Player& operator=(Player const&) = default;
        Player(Player&&) = default;
        Player& operator=(Player&&) = default;

        Stack getAmount(Stack amount) const;
        void putAmount(Stack amt) const;
        void dealHoleCards(HoleCards const&);
        bool hasHoleCards() const;
        HoleCards getHoleCards() const;
        Decision decide(Dealer& pot, Board const& board, HandHistory& handHistory);
        bool ready(Stack currentBet, Board const& board, HandHistory const& handHistory) const;
        void setPlayerIndex(int index, size_t size);
        Position getPosition() const;
        Stack getStack() const;
        int getNumber() const;
        std::string getName() const;
        bool isAllin(Stack bet) const;
    private:
        mutable Stack stack_;
        std::shared_ptr<DecisionEngine> engine_;
        HoleCards holeCards_;
        std::pair<int, size_t> pos_{0,2}; // to deduce the position;
        int nb_;
};

}