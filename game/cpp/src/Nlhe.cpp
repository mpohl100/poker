#include "Nlhe.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "DecisionEngine.h"
#include "Player.h"
#include "Hand.h"
#include <algorithm>

namespace game52{

Nlhe52::Nlhe52(int nb_players)
{
    for(int i = 0; i < nb_players; ++i)
        players_.push_back(Player(Stack(100*bigBlind), DecisionEngine{}));
    // Players in hand 0: dealer, 1: small blind, 2: big blind
    for(auto& player : players_)
        playersInHand_.push_back(&player);
}

void Nlhe52::playHand()
{
    // shuffle
    deck_.shuffle();
    // move dealer button
    std::rotate(playersInHand_.begin(), playersInHand_.begin()+1, playersInHand_.end());
    // post blinds and antes
    Pot pot;
    Board board;
    HandHistory handHistory;
    pot.putAmount(small_blind_player().getAmount(smallBlind));
    pot.putAmount(big_blind_player().getAmount(bigBlind));

    // deal hands
    for(auto& player : players_)
        player.dealHoleCards(deck_.getHoleCards());

    // pre-flop play
    if(bool finished = playRound(3, Stack(bigBlind), board, handHistory, pot); finished)
        return;

    // flop play
    board.dealFlop(deck_.getFlop());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
        return;

    // turn play
    board.dealTurn(deck_.getCard());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
        return;

    // river play
    board.dealRiver(deck_.getCard());
    if(bool finished = playRound(0, Stack(0), board, handHistory, pot); finished)
        return;

    // show down
    std::vector<Hand> hands;
    for(auto& player : players_)
        if( player.hasHoleCards() )
            hands.push_back(Hand(&player, player.getHoleCards(), board));
    std::sort(hands.begin(), hands.end(), [](const auto& l, const auto& r){ return compareHands(l,r) == 1;});
    int indexFirstLoser = 0;
    for(size_t i = 1; i < hands.size(); ++i)
        if(compareHands(hands[i-1], hands[i]) != 0)
        {
            indexFirstLoser = i;
            break;
        }
    Stack amount = pot.getAmount() / indexFirstLoser;
    for(int i = 0; i < indexFirstLoser; ++i)
        hands[0].player_->putAmount(amount);
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

bool Nlhe52::playRound(int starting_pos, Stack currentBet, Board const& board, HandHistory& handHistory, Pot& pot)
{
    int i = 0;
    while(not ready(currentBet, board))
    {
        int nb = 0;
        for(auto& player : playersInHand_)
        {
            if( not player->hasHoleCards() or (i == 0 and nb++ < starting_pos))
                continue;
            currentBet = player->decide(pot, board, handHistory);
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
