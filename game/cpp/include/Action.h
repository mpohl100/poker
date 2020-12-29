#pragma once

#include "Stack.h"
#include "Board.h"
#include "HoleCards.h"
#include "Player.h"

namespace game52{

class HandAction{
public:
    HandAction() = default;
    virtual ~HandAction() = default;
    HandAction(HandAction const&) = default;
    HandAction& operator=(HandAction const&) = default;
    HandAction(HandAction&&) = default;
    HandAction& operator=(HandAction&&) = default;

    virtual std::string toString() const = 0;    
    game52::Player player;
};

class BettingAction : public HandAction{
public: 
    BettingAction(Street street);
    BettingAction() = default;
    virtual ~BettingAction() = default;
    BettingAction(BettingAction const&) = default;
    BettingAction& operator=(BettingAction const&) = default;
    BettingAction(BettingAction&&) = default;
    BettingAction& operator=(BettingAction&&) = default;
    
    std::string toString() const override;
    Stack previousBet = 0;
    Stack nextBet = 0;
    Stack toCall = 0;
    Decision decision;
    Street street;
};

class DealingAction : public HandAction{
public:
    DealingAction(HoleCards const& holeCards);
    DealingAction() = default;
    virtual ~DealingAction() = default;
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
    virtual ~BoardAction() = default;
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
    virtual ~SeatingAction() = default;
    SeatingAction(SeatingAction const&) = default;
    SeatingAction& operator=(SeatingAction const&) = default;
    SeatingAction(SeatingAction&&) = default;
    SeatingAction& operator=(SeatingAction&&) = default;
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