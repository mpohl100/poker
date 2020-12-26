#include "HandHistory.h"

#include <ranges>

namespace game52{

void HandHistory::logAction(std::unique_ptr<HandAction> handAction)
{
    actions_.push_back(std::move(handAction));
}

BettingAction HandHistory::getLastBet(Street street)
{
    auto bets = actions_ 
                        | std::ranges::views::reverse 
                        | std::ranges::views::filter([street](const auto& action)
                        {
                            if( auto bet = dynamic_cast<BettingAction*>(action.get()); bet )
                                return bet->street == street;
                            return false;
                        })
                        | std::ranges::views::take(1);
    auto lastBet = std::ranges::begin(bets);
    if(lastBet != std::ranges::end(bets))
        return *dynamic_cast<BettingAction*>((*lastBet).get());
    return BettingAction(street);
}


std::string HandHistory::toString() const
{
    std::string str;
    for(const auto& action : actions_)
        str += action->toString() + '\n';
    return str;
}

}