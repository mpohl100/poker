#pragma once

#include "Stack.h"

#include <map>

namespace game52{

class Player;

class Pot{
    public:
        void putAmount(Player* player, Stack amt);
        int getAmount() const;
        Stack get();
        std::string toString() const;
    private:
        std::map<Player*, Stack> bets_;
        Stack amount_ = 0;
};

}