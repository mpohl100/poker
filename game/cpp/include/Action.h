#pragma once

#include "Stack.h"
#include "Board.h"

#include <memory>

namespace game52{

class Player;

enum class Action{
    Fold,
    Check,
    Call,
    Raise,
};

class HandAction{
    public: 
        std::shared_ptr<Player> player;
        Stack previousBet;
        Stack nextBet;
        Street street;
};

}