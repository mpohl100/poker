#pragma once

#include "Stack.h"

namespace game52{

class Player{
    public:
        Player(Stack stack);
        Player() = default;
        Player(Player const&) = default;
        Player& operator=(Player const&) = default;
        Player(Player&&) = default;
        Player& operator=(Player&&) = default;

        int get_amount(int amount);

    private:
        Stack stack_;
};

}