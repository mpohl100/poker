#include "Nlhe.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include <algorithm>

namespace game52{

Nlhe52::Nlhe52(int nb_players)
{
    for(int i = 0; i < nb_players; ++i)
        players_.push_back(Player{});
}

void Nlhe52::playHand()
{
    // shuffle
    deck_.shuffle();
    // move dealer button
    std::rotate(players_.begin(), players_.end(), players_.begin()+1);
    // post blinds and antes
    Pot pot;
    Board board;
    HandHistory handHistory;
    pot.put_amount(small_blind_player().get_amount(small_blind_));
    pot.put_amount(big_blind_player().get_amount(big_blind_));

    // deal hands
    for(auto& player : players_)
        player.dealHoleCards(deck_.getHoleCards());

    // pre-flop play
    Stack currentBet = Stack(big_blind_);

    for(auto& player : players_)
    {
        if( not player.hasHoleCards() )
            continue;
        currentBet = player.decide( pot, board, handHistory);
    }

    // flop play

    // turn play

    // river play

    // show down
}

Player& Nlhe52::small_blind_player()
{
    if(players_.size() == 2)
        return players_[0];
    return players_[1];
}

Player& Nlhe52::big_blind_player()
{
    if(players_.size() == 2)
        return players_[1];
    return players_[2];
}

}