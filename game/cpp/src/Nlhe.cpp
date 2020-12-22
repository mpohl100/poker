#include "Nlhe.h"
#include "Pot.h"

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
    dealer_ += 1;
    if( dealer_ >= players_.size() )
        dealer_ = 0;
    // post blinds and antes
    Pot pot;
    pot.put_amount(small_blind_player().get_amount(small_blind_));
    pot.put_amount(big_blind_player().get_amount(big_blind_));

    // deal hands
    //for(auto& player : players_)
    //    player.dealHoleCards(deck_.getHoleCards());

    // pre-flop play

    // flop play

    // turn play

    // river play

    // show down
}

Player& Nlhe52::small_blind_player()
{
    size_t pos = dealer_ + 1;
    if( pos >= players_.size() )
        pos = pos / players_.size();
    return players_[pos];
}

Player& Nlhe52::big_blind_player()
{
    size_t pos = dealer_ + 2; 
    if( pos >= players_.size() )
        pos = pos / players_.size();
    return players_[pos];
}

}