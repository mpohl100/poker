#pragma once

#include "Deck52.h"
#include "Player.h"

namespace game52{

class Nlhe52{
public:
    Nlhe52(int nb_players);
    Nlhe52() = default;
    Nlhe52(Nlhe52 const&) = default;
    Nlhe52& operator=(Nlhe52 const&) = default;
    Nlhe52(Nlhe52&&) = default;
    Nlhe52& operator=(Nlhe52&&) = default;

    void playHand();
private:
    Player& small_blind_player();
    Player& big_blind_player();
    bool ready(Stack amt, Board const& board);
    bool playRound(int starting_pos, Stack currentBet, Board const& board, HandHistory& HandHistory, Pot& pot);

    Deck52 deck_;
    size_t dealer_ = 0;
    std::vector<Player> players_;
    std::vector<Player*> playersInHand_;
    int smallBlind = 10;
    int bigBlind = smallBlind*2;
};

}