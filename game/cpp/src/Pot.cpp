#include "Pot.h"
#include "Player.h"

namespace game52 {

void Pot::putAmount(const Player* player, Stack amt)
{
    if( amt == Stack(0))
        return;
    amount_ += amt;
    bets_[player] += amt;
}

int Pot::getAmount() const
{
    return amount_.amount_;
}

Stack Pot::get()
{
    auto amount = amount_;
    amount_ = 0;
    return amount;
}

std::string Pot::toString() const
{
    return "main pot";
}

std::map<const Player*, Stack> const& Pot::getBets() const
{
    return bets_;
}


}