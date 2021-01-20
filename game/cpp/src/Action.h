#pragma once

#include "Stack.h"
#include "Board.h"
#include "HoleCards.h"
#include "Player.h"
#include "Hand.h"
#include "Pot.h"

namespace game52{

class HandAction{
public:
    HandAction(Player const& player);
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
    BettingAction(Player const& player, Street street);
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

    static BettingAction
    create( Player const& player, Street street, 
            Stack nextBet, Stack previousBet, 
            Stack toCall, Decision decision);
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
    ShowdownAction(Player const& player, std::optional<Hand> const& hand);
    ShowdownAction() = default;
    ShowdownAction(ShowdownAction const&) = default;
    ShowdownAction& operator=(ShowdownAction const&) = default;
    ShowdownAction(ShowdownAction&&) = default;
    ShowdownAction& operator=(ShowdownAction&&) = default;
    std::string toString() const override;
    std::optional<Hand> hand;
};

class PotAction : public HandAction{
public:
    PotAction(Player const& player, Stack amount, Pot const& pot);
    PotAction() = default;
    PotAction(PotAction const&) = default;
    PotAction& operator=(PotAction const&) = default;
    PotAction(PotAction&&) = default;
    PotAction& operator=(PotAction&&) = default;
    std::string toString() const override;
    Stack amount;
    Pot pot;
};



class SummaryAction : public HandAction{
public:
    std::string toString() const override;
};

}