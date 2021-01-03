#include "DecisionEngine.h"
#include "Board.h"
#include "Player.h"
#include "Dealer.h"

namespace game52{

Stack getToCall(Options const& options)
{
    auto toCallIt = std::find_if(options.options.begin(), options.options.end(), [](const auto& opt){ return opt.first == Decision::Call;});
    if(toCallIt == options.options.end())
        return Stack(0);
    return toCallIt->second.first;
}

BettingAction
DecisionEngine::decide( Dealer const& dealer, 
                        Board const& board, 
                        Player& hero)
{    
    auto options = dealer.getOptions(hero);
    if(options.options.size() == 0)
        throw std::runtime_error("player is asked to decide, although there are no more actions necessary from him");
    int index = rand() % options.options.size();
    auto [decision, bettingRange] = options.options[index];
    Stack nextBet = bettingRange.first + (bettingRange.second - bettingRange.first) * (rand() % 100) / 100;
    BettingAction bet(hero, board.street());
    bet.nextBet = nextBet;
    bet.decision = decision;
    bet.previousBet = dealer.getCurrentBet(hero);
    bet.toCall = getToCall(options);
    return bet;
}

}