#include "HandHistory.h"

#include <range/v3/all.hpp>
#include <iostream>

namespace game52{

void HandHistory::logAction(std::unique_ptr<HandAction> handAction)
{
    actions_.push_back(std::move(handAction));
}

std::optional<BettingAction> HandHistory::getLastBet(Street street, Player const* player) const
{
    auto bets = actions_ 
                        | ranges::views::reverse 
                        | ranges::views::filter([&](const auto& action)
                        {
                            if( auto bet = dynamic_cast<BettingAction*>(action.get()); bet )
                            {
                                if(not player)
                                    return bet->street == street;
                                return bet->street == street and bet->player.getNumber() == player->getNumber();
                            }
                            return false;
                        })
                        | ranges::views::take(1);
    auto lastBet = std::ranges::begin(bets);
    if(lastBet != std::ranges::end(bets))
    {
        auto bet = *dynamic_cast<BettingAction*>((*lastBet).get());
        return bet;
    }
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