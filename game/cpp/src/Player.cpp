#include "Player.h"
#include "HandHistory.h"
#include "Pot.h"
#include "DecisionEngine.h"
#include "Dealer.h"

#include <iostream>

namespace game52{

std::string toString(Position pos)
{
    switch(pos){
        case Position::Button: return "Button";
        case Position::SmallBlind: return "Small Blind";
        case Position::BigBlind: return "Big Blind";
        case Position::UTG: return "UTG";
        case Position::UTGp1: return "UTG+1";
        case Position::UTGp2: return "UTG+2";
        case Position::Lojack: return "Lojack";
        case Position::Hijack: return "Hijack";
        case Position::Cutoff: return "Cutoff";
    }
    return "Unknown Position";
}

Player::Player(Stack stack, DecisionEngine const& engine, int nb)
    : stack_(stack)
    , engine_(std::make_shared<DecisionEngine>(engine))
    , nb_(nb)
{}

Stack Player::getAmount(Stack amt) const
{
    return stack_.getAmount(amt); 
}

void Player::putAmount(Stack amt) const
{
    stack_ += amt; 
}

void Player::dealHoleCards(HoleCards const& hole_cards)
{
    holeCards_ = hole_cards;
}

bool Player::hasHoleCards() const
{
    return not holeCards_.empty();
}


HoleCards Player::getHoleCards() const
{
    return holeCards_;
}

Decision 
Player::decide(Dealer& dealer, Board const& board, HandHistory& handHistory)
{
    BettingAction next = engine_->decide(dealer, board, *this);
    handHistory.logAction(std::make_unique<BettingAction>(next));
    dealer.acceptBet(*this, next);
    // only fold, if it has been bet before
    if(next.decision == Decision::Fold)
        holeCards_ = {};
    return next.decision;
}

bool Player::ready(Stack currentBet, Board const& board, HandHistory const& handHistory) const
{
    if(not hasHoleCards())
        return true;
    std::optional<BettingAction> lastPlayerBet = handHistory.getLastBet(board.street(), this);
    if(lastPlayerBet)
        return lastPlayerBet->nextBet == currentBet;
    return false;
}

void Player::setPlayerIndex(int index, size_t size)
{
    pos_.first = index;
    pos_.second = size;
}

Position Player::getPosition() const
{
    switch(pos_.second){
        case 2:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::BigBlind;
            }; break;
        case 3:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
            }; break;
        case 4:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::Cutoff;
            }; break;
        case 5:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::Hijack;
                case 4: return Position::Cutoff;
            }; break;
        case 6:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::Lojack;
                case 4: return Position::Hijack;
                case 5: return Position::Cutoff;
            }; break;
        case 7: 
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::UTGp2;
                case 4: return Position::Lojack;
                case 5: return Position::Hijack;
                case 6: return Position::Cutoff;
            }; break;
        case 8:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::UTGp1;
                case 4: return Position::UTGp2;
                case 5: return Position::Lojack;
                case 6: return Position::Hijack;
                case 7: return Position::Cutoff;
            }; break;
        case 9:
            switch(pos_.first){
                case 0: return Position::Button;
                case 1: return Position::SmallBlind;
                case 2: return Position::BigBlind;
                case 3: return Position::UTG;
                case 4: return Position::UTGp1;
                case 5: return Position::UTGp2;
                case 6: return Position::Lojack;
                case 7: return Position::Hijack;
                case 8: return Position::Cutoff;
            }; break;
        default: throw std::runtime_error("wrong position indeces: index " + std::to_string(pos_.first) + " size " + std::to_string(pos_.second) );
    }
    return Position::Button;
}

Stack Player::getStack() const
{
    return stack_;
}

int Player::getNumber() const
{
    return nb_;
}


std::string Player::getName() const
{
    return "Player " + std::to_string(nb_);
}

bool Player::isAllin(Stack bet) const
{
    return stack_ <= bet;
}

}