#include "HandHistory.h"

#include <ranges>
#include <iostream>

namespace game52{

void HandHistory::logAction(std::unique_ptr<HandAction> handAction)
{
    actions_.push_back(std::move(handAction));
}

std::optional<BettingAction> HandHistory::getLastBet(Street street, const Player* player) const
{
    auto bets = actions_ 
                        | std::ranges::views::reverse 
                        | std::ranges::views::filter([&](const auto& action)
                        {
                            if( auto bet = dynamic_cast<BettingAction*>(action.get()); bet )
                            {
                                if(not player)
                                    return bet->street == street;
                                return bet->street == street and bet->player.get() == player;
                            }
                            return false;
                        })
                        | std::ranges::views::take(1);
    auto lastBet = std::ranges::begin(bets);
    std::cout << "before returning last bet\n";
    if(lastBet != std::ranges::end(bets))
        return *dynamic_cast<BettingAction*>((*lastBet).get());
    return std::nullopt;
}


std::string HandHistory::toString() const
{
    std::string str;
    for(const auto& action : actions_)
        str += action->toString() + '\n';
    return str;
}

}