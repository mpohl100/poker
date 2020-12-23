#pragma once

#include "Card52.h"
#include <vector>

namespace game52{

class HoleCards;
class Board;
class Player;

class Hand
{
public:
    Hand(Player* player, HoleCards const& holeCards, Board const& board);
    Hand() = default;
    Hand(Hand const&) = default;
    Hand& operator=(Hand const&) = default;
    Hand(Hand&&) = default;
    Hand& operator=(Hand&&) = default;
    
    Player* player_;
private:
    std::vector<Card52> cards_;
};

int compareHands(Hand left, Hand right);

}