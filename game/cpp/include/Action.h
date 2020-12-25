#pragma once

#include "Stack.h"
#include "Board.h"

#include <memory>

namespace game52{

class Player;

class HandAction{
public: 
    std::string toString() const;

    std::shared_ptr<Player> player;
    Stack previousBet;
    Stack nextBet;
    Street street;


};

}