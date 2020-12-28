#include "Nlhe.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "DecisionEngine.h"
#include "Player.h"
#include "Hand.h"

#include <range/v3/all.hpp>
#include <algorithm>
#include <iostream>

namespace game52{

Nlhe52::Nlhe52(int nb_players)
{
    for(int i = 0; i < nb_players; ++i)
        players_.push_back(Player(Stack(100*bigBlind), DecisionEngine{}, i+1));
    // Players in hand 0: dealer, 1: small blind, 2: big blind
    for(auto& player : players_)
        playersInHand_.push_back(player);
}

template<class T>
auto offset(std::vector<T> const& rng, size_t offset)
{ 
    size_t N = ranges::size(rng);
    if(offset >= N)
        throw std::runtime_error("wrong offset in offset range creation: " + std::to_string(offset));
    return rng | ranges::view::cycle | ranges::view::take(N+offset) | ranges::view::drop(offset);
}

void Nlhe52::playHand()
{
    // shuffle
    deck_.shuffle();
    // move dealer button
    std::rotate(playersInHand_.begin(), playersInHand_.begin()+1, playersInHand_.end());

    // post position indeces
    for(size_t i = 0; i < playersInHand_.size(); ++i)
        playersInHand_[i].get().setPlayerIndex(i, playersInHand_.size());

    HandHistory handHistory;
    for(auto& player : players_){
        SeatingAction seatingAction;
        seatingAction.player.reset(&player);
        seatingAction.startingStack = player.getStack();
        handHistory.logAction(std::make_unique<SeatingAction>(seatingAction));
    }
    // post blinds and antes
    Pot pot;
    Board board;
    BettingAction sbAction(Preflop);
    sbAction.player.reset(&smallBlindPlayer());
    sbAction.nextBet = smallBlind;
    handHistory.logAction(std::make_unique<BettingAction>(sbAction));
    pot.putAmount(smallBlindPlayer().getAmount(smallBlind));
    BettingAction bbAction(Preflop);
    bbAction.player.reset(&bigBlindPlayer());
    bbAction.nextBet = bigBlind;
    handHistory.logAction(std::make_unique<BettingAction>(bbAction));
    pot.putAmount(bigBlindPlayer().getAmount(bigBlind));

    BoardAction preflopAction;
    preflopAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(preflopAction));

    // deal hands
    for(auto& player : offset(playersInHand_,1)){
        HoleCards holeCards = deck_.getHoleCards();  
        player.get().dealHoleCards(holeCards);
        DealingAction dealingAction;
        dealingAction.player.reset(&player.get());
        dealingAction.holeCards = holeCards;
        handHistory.logAction(std::make_unique<DealingAction>(dealingAction));
    }

    // pre-flop play
    // only with 4 players or more UTG gets to act first, otherwise the dealer always acts first.
    size_t firstToAct = players_.size() > 3 ? 3 : 0;
    if(bool finished = playRound(firstToAct, pot, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // flop play
    board.dealFlop(deck_.getFlop());
    BoardAction flopAction;
    flopAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(flopAction));


    if(bool finished = playRound(1, pot, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // turn play
    board.dealTurn(deck_.getCard());
    BoardAction turnAction;
    turnAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(turnAction));

    if(bool finished = playRound(1, pot, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // river play
    board.dealRiver(deck_.getCard());
    BoardAction riverAction;
    riverAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(riverAction));

    if(bool finished = playRound(1, pot, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // show down
    std::vector<Hand> hands;
    for(auto& player : playersInHand_ | ranges::view::filter([](const auto& p){ return p.get().hasHoleCards();}))
        hands.push_back(Hand(&player.get(), player.get().getHoleCards(), board));
    std::sort(hands.begin(), hands.end(), [](const auto& l, const auto& r){ return compareHands(l,r) == 1;});
    Stack amount = pot.getAmount();
    hands[0].player_->putAmount(amount);

    std::cout << handHistory.toString() << '\n';
}

Player& Nlhe52::smallBlindPlayer()
{
    if(players_.size() == 2)
        return players_[0];
    return players_[1];
}

Player& Nlhe52::bigBlindPlayer()
{
    if(players_.size() == 2)
        return players_[1];
    return players_[2];
}

bool Nlhe52::ready(Stack currentBet, Board const& board, HandHistory const& handHistory)
{
    return std::all_of(players_.begin(), players_.end(), 
                         [currentBet, &board, &handHistory](auto const& p){ 
                                bool ready = p.ready(currentBet, board, handHistory);
                                return ready;
                             });
}

bool Nlhe52::playRound(size_t starting_pos, Pot& pot, Board const& board, HandHistory& handHistory)
{
    while(true){
        size_t firstToAsk = starting_pos;
        bool newRaiser = false;
        size_t nb = 0;
        for(auto& player : offset(playersInHand_, firstToAsk))
        {
            if( not player.get().hasHoleCards() )
                continue;
            DecisionEngine::Decision decision = player.get().decide(pot, board, handHistory);
            if(decision == DecisionEngine::Decision::Raise)
            {
                newRaiser = true;
                firstToAsk = nb+1;
                if(firstToAsk > playersInHand_.size())
                    firstToAsk -= playersInHand_.size();
                break;
            }
            nb++;
        }
        if(not newRaiser)
            break;
    }
    // if only one player has hole cards after the betting round, the hand is finished
    if(std::count_if(players_.begin(), players_.end(), [](const auto& p){ return p.hasHoleCards();}) == 1)
    {
        auto it = std::find_if(players_.begin(), players_.end(),[](const auto& p){return p.hasHoleCards();} );
        it->putAmount(pot.get());
        return true; // hand finished
    }
    return false;
}

}
