#include "Nlhe.h"
#include "Pot.h"
#include "Board.h"
#include "HandHistory.h"
#include "DecisionEngine.h"
#include "Player.h"
#include "Hand.h"
#include "Dealer.h"

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
auto offset(std::vector<T> const& rng, size_t offset, size_t N = 0)
{ 
    if(N == 0)
        N = ranges::size(rng);
    if(offset > N)
        throw std::runtime_error("wrong offset in offset range creation: " + std::to_string(offset));
    return rng | ranges::views::cycle | ranges::views::take(N+offset) | ranges::views::drop(offset);
}

void Nlhe52::playHand()
{
    // rebuy
    for(auto& player : playersInHand_)
        if(player.get().getStack() == Stack(0))
            player.get().putAmount(Stack(bigBlind)*100);
    Stack allPlayers = std::accumulate(playersInHand_.begin(), playersInHand_.end(),Stack(0), 
                    [](Stack stack, const auto& p){ return stack + p.get().getStack();});
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
        seatingAction.player = player;
        handHistory.logAction(std::make_unique<SeatingAction>(seatingAction));
    }
    // post blinds and antes
    Dealer dealer{playersInHand_, Stack(bigBlind)};
    Board board;

    BettingAction sbAction(smallBlindPlayer(), Preflop);
    sbAction.nextBet = Stack(smallBlind) < smallBlindPlayer().getStack() ? Stack(smallBlind) : smallBlindPlayer().getStack();
    sbAction.decision = Decision::Raise;
    handHistory.logAction(std::make_unique<BettingAction>(sbAction));
    dealer.acceptBet(smallBlindPlayer(), sbAction);

    BettingAction bbAction(bigBlindPlayer(),Preflop);
    bbAction.nextBet = Stack(bigBlind) < bigBlindPlayer().getStack() ? Stack(bigBlind) : bigBlindPlayer().getStack();
    bbAction.decision = Decision::Raise;
    handHistory.logAction(std::make_unique<BettingAction>(bbAction));
    dealer.acceptBet(bigBlindPlayer(), bbAction);


    BoardAction preflopAction;
    preflopAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(preflopAction));

    // deal hands
    for(auto& player : offset(playersInHand_,1)){
        HoleCards holeCards = deck_.getHoleCards();  
        player.get().dealHoleCards(holeCards);
        DealingAction dealingAction;
        dealingAction.player = player.get();
        dealingAction.holeCards = holeCards;
        handHistory.logAction(std::make_unique<DealingAction>(dealingAction));
    }

    // pre-flop play
    // only with 4 players or more UTG gets to act first, otherwise the dealer always acts first.
    size_t firstToAct = players_.size() > 3 ? 3 : 0;
    if(auto[ finished, lastRaiser] = playRound(firstToAct, dealer, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        std::cout << allPlayers.toString() << '\n';
        auto allPlayersAfterHand = std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();});
        std::cout << allPlayersAfterHand.toString() << '\n';
        
        assert(allPlayers == std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();}));
        return;
    }

    // flop play
    board.dealFlop(deck_.getFlop());
    BoardAction flopAction;
    flopAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(flopAction));


    if(auto[ finished, lastRaiser] = playRound(1, dealer, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        std::cout << allPlayers.toString() << '\n';
        auto allPlayersAfterHand = std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();});
        std::cout << allPlayersAfterHand.toString() << '\n';
        assert(allPlayers == std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();}));
        return;
    }

    // turn play
    board.dealTurn(deck_.getCard());
    BoardAction turnAction;
    turnAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(turnAction));

    if(auto[ finished, lastRaiser] = playRound(1, dealer, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        std::cout << allPlayers.toString() << '\n';
        auto allPlayersAfterHand = std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();});
        std::cout << allPlayersAfterHand.toString() << '\n';
        assert(allPlayers == std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();}));
        return;
    }

    // river play
    board.dealRiver(deck_.getCard());
    BoardAction riverAction;
    riverAction.board = board;
    handHistory.logAction(std::make_unique<BoardAction>(riverAction));

    if(auto[ finished, lastRaiser] = playRound(1, dealer, board, handHistory); finished)
    {
        std::cout << handHistory.toString() << '\n';
        std::cout << allPlayers.toString() << '\n';
        auto allPlayersAfterHand = std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();});
        std::cout << allPlayersAfterHand.toString() << '\n';
        assert(allPlayers == std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();}));
        return;
    }
    std::cout << handHistory.toString() << '\n';
    std::cout << allPlayers.toString() << '\n';
    auto allPlayersAfterHand = std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();});
    std::cout << allPlayersAfterHand.toString() << '\n';
    assert(allPlayers == std::accumulate(playersInHand_.begin(), playersInHand_.end(), Stack(0),
                            [](Stack stack, const auto& p){ return stack + p.get().getStack();}));
}

Player& Nlhe52::smallBlindPlayer()
{
    if(players_.size() == 2)
        return playersInHand_[0];
    return playersInHand_[1];
}

Player& Nlhe52::bigBlindPlayer()
{
    if(players_.size() == 2)
        return playersInHand_[1];
    return playersInHand_[2];
}

bool Nlhe52::ready(Stack currentBet, Board const& board, HandHistory const& handHistory)
{
    return std::all_of(players_.begin(), players_.end(), 
                         [currentBet, &board, &handHistory](auto const& p){ 
                                bool ready = p.ready(currentBet, board, handHistory);
                                return ready;
                             });
}

std::pair<bool,int> 
Nlhe52::playRound(size_t starting_pos, Dealer& dealer, Board const& board, HandHistory& handHistory)
{
    size_t firstToAsk = starting_pos;
    size_t N = playersInHand_.size();
    while(true){
        bool newRaiser = false;
        size_t nb = 0;
        for(auto& player : offset(playersInHand_, firstToAsk, N))
        {
            nb++;
            if( dealer.getOptions(player.get()).options.empty())
                continue;
            Decision decision = player.get().decide(dealer, board, handHistory);
            if(decision == Decision::Raise)
            {
                newRaiser = true;
                firstToAsk += nb;
                if(firstToAsk >= playersInHand_.size())
                    firstToAsk -= playersInHand_.size();
                break;
            }
        }
        N = playersInHand_.size() - 1;
        if(not newRaiser)
            break;
    }
    dealer.rakeIn();
    int raiser = firstToAsk - 1;
    if(raiser < 0)
        raiser += int(playersInHand_.size());
    return {dealer.awardPots(board, handHistory), raiser};
}

}
