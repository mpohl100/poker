#include "Nlhe.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "DecisionEngine.h"
#include "Player.h"
#include "Hand.h"

#include <algorithm>
#include <iostream>

namespace game52{

Nlhe52::Nlhe52(int nb_players)
{
    for(int i = 0; i < nb_players; ++i)
        players_.push_back(Player(Stack(100*bigBlind), DecisionEngine{}));
    // Players in hand 0: dealer, 1: small blind, 2: big blind
    for(auto& player : players_)
        playersInHand_.push_back(player);
}

void Nlhe52::playHand()
{
    // shuffle
    deck_.shuffle();
    std::cout << "shuffled deck.\n";
    // move dealer button
    std::rotate(playersInHand_.begin(), playersInHand_.begin()+1, playersInHand_.end());
    std::cout << "rotated players\n";
    // post position indeces
    for(size_t i = 0; i < playersInHand_.size(); ++i)
        playersInHand_[i].get().setPlayerIndex(i, playersInHand_.size());
    // post blinds and antes
    Pot pot;
    Board board;
    HandHistory handHistory;
    pot.putAmount(smallBlindPlayer().getAmount(smallBlind));
    pot.putAmount(bigBlindPlayer().getAmount(bigBlind));

    // deal hands
    for(auto& player : players_)
        player.dealHoleCards(deck_.getHoleCards());

    // pre-flop play
    // only with 4 players or more UTG gets to act first, otherwise the dealer always acts first.
    int firstToAct = players_.size() > 3 ? 3 : 0;
    if(bool finished = playRound(firstToAct, Stack(bigBlind), board, handHistory, pot); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // flop play
    board.dealFlop(deck_.getFlop());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // turn play
    board.dealTurn(deck_.getCard());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // river play
    board.dealRiver(deck_.getCard());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
    {
        std::cout << handHistory.toString() << '\n';
        return;
    }

    // show down
    std::vector<Hand> hands;
    for(auto& player : players_)
        if( player.hasHoleCards() )
            hands.push_back(Hand(&player, player.getHoleCards(), board));
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
                                std::cout << "ready\n";
                                return ready;
                             });
}

bool Nlhe52::playRound(int starting_pos, Stack currentBet, Board const& board, HandHistory& handHistory, Pot& pot)
{
    int i = 0;
    while(not ready(currentBet, board, handHistory))
    {
        int nb = 0;
        for(auto& player : playersInHand_)
        {
            if( not player.get().hasHoleCards() or (i == 0 and nb++ < starting_pos))
                continue;
            currentBet = player.get().decide(pot, board, handHistory);
        }
        i++;
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
