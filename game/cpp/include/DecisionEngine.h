#pragma once

#include "Action.h"

#include <optional>

namespace game52{

class Dealer;
class Board;
class Player;

class DecisionEngine{
public:
    BettingAction decide(Dealer const& dealer, Board const& board, Player& hero);
};

}