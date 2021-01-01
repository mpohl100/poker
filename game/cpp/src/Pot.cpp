#include "Pot.h"
#include "Player.h"

namespace game52 {

void Pot::putAmount(Player* player, Stack amt)
{
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


}