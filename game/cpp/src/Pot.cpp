#include "Pot.h"

namespace game52 {

void Pot::putAmount(Stack amt)
{
    amount_ += amt;
}

Stack Pot::get()
{
    auto amount = amount_;
    amount_ = 0;
    return amount;
}


}