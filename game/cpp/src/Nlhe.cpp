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
    pot.putAmount(small_blind_player().getAmount(small_blind_));
    pot.putAmount(big_blind_player().getAmount(big_blind_));

    // deal hands
    for(auto& player : players_)
        player.dealHoleCards(deck_.getHoleCards());

    // pre-flop play
    if(bool finished = playRound(Stack(big_blind_), board, handHistory, pot); finished)
        return;

    // flop play
    board.dealFlop(deck_.getFlop());
    if(bool finished = playRound(Stack(0), board, handHistory, pot); finished)
        return;

    // turn play
    board.dealTurn(deck_.getCard());
    if(bool finished = playRound(Stack(0), board, handHistory, pot); finished)
        return;
    // river play
    board.dealRiver(deck_.getCard());
    if(bool finished = playRound(Stack(0), board, handHistory, pot); finished)
        return;

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

bool Nlhe52::ready(Stack amt, Board const& board)
{
    return std::all_of(players_.begin(), players_.end(), 
                         [amt, &board](auto const& p){ return p.ready(amt, board);});
}

bool Nlhe52::playRound(Stack currentBet, Board const& board, HandHistory& handHistory, Pot& pot)
{
    while(not ready(currentBet, board))
        for(auto& player : players_)
        {
            if( not player.hasHoleCards() )
                continue;
            currentBet = player.decide(pot, board, handHistory);
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
