#pragma once

#include "Stack.h"

#include <map>

namespace game52{

class Player;

class Pot{
    public:
        void putAmount(const Player* player, Stack amt);
        int getAmount() const;
        Stack get();
        std::string toString() const;
        std::map<const Player*, Stack> const& getBets() const;
    private:
        std::map<const Player*, Stack> bets_;
        Stack amount_ = 0;
};

}