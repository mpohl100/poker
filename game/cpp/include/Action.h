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
    DealingAction(HoleCards const& holeCards);
    DealingAction() = default;
    DealingAction(DealingAction const&) = default;
    DealingAction& operator=(DealingAction const&) = default;
    DealingAction(DealingAction&&) = default;
    DealingAction& operator=(DealingAction&&) = default;
    std::string toString() const override;

    HoleCards holeCards;
};

class BoardAction : public HandAction{
public:
    BoardAction() = default;
    BoardAction(BoardAction const&) = default;
    BoardAction& operator=(BoardAction const&) = default;
    BoardAction(BoardAction&&) = default;
    BoardAction& operator=(BoardAction&&) = default;
    std::string toString() const override;

    Board board;
};

class SeatingAction : public HandAction{
public:
    SeatingAction() = default;
    SeatingAction(SeatingAction const&) = default;
    SeatingAction& operator=(SeatingAction const&) = default;
    SeatingAction(SeatingAction&&) = default;
    SeatingAction& operator=(SeatingAction&&) = default;
    std::string toString() const override;
    Stack startingStack;
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