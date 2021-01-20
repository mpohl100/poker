#pragma once

#include "Deck52.h"
#include "Player.h"

namespace game52{

class Dealer;

class Nlhe52{
public:
    Nlhe52(int nb_players);
    Nlhe52(Nlhe52 const&) = default;
    Nlhe52& operator=(Nlhe52 const&) = default;
    Nlhe52(Nlhe52&&) = default;
    Nlhe52& operator=(Nlhe52&&) = default;

    void playHand();
private:
    Player& smallBlindPlayer();
    Player& bigBlindPlayer();
    bool ready(Stack currentBet, Board const& board, HandHistory const& handHistory);
    std::pair<bool, int> playRound(size_t starting_pos, Dealer& dealer, Board const& board, HandHistory& HandHistory);

    Deck52 deck_;
    std::vector<Player> players_;
    std::vector<std::reference_wrapper<Player>> playersInHand_;
    int smallBlind = 10;
    int bigBlind = smallBlind*2;
};

}