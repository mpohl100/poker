#pragma once

#include "Stack.h"
#include "Board.h"
#include "HoleCards.h"

#include <memory>

namespace game52{

class Player;

class HandAction{
public:
    virtual std::string toString() const = 0;    
    
    std::shared_ptr<Player> player;
};

class BettingAction : public HandAction{
public: 
    BettingAction(Street street);
    BettingAction() = default;
    BettingAction(BettingAction const&) = default;
    BettingAction& operator=(BettingAction const&) = default;
    BettingAction(BettingAction&&) = default;
    BettingAction& operator=(BettingAction&&) = default;
    
    std::string toString() const override;
    Stack previousBet = 0;
    Stack nextBet = 0;
    Street street;
};

class DealingAction : public HandAction{
public:
    std::string toString() const override;

    HoleCards holeCards;
};

class BoardAction : public HandAction{
public:
    std::string toString() const override;

    Board board;
};

class SeatingAction : public HandAction{
public:
    std::string toString() const override;
};

class ShowdownAction : public HandAction{
public:
    std::string toString() const override;
};

class SummaryAction : public HandAction{
public:
    std::string toString() const override;
};

}