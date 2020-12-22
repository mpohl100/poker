#include "Stack.h"

namespace game52{

Stack Stack::getAmount(Stack amt){
    if(amt.amount_ > amount_)
    {
        amount_ = 0;
        return *this;
    }
    else
    {
        amount_ -= amt.amount_;
        return amt;
    }
}

Stack& Stack::operator+=(Stack const& other)
{
    amount_ += other.amount_;
    return *this;
}

bool operator==(Stack left, Stack right)
{
    return left.amount_ == right.amount_;
}

bool operator!=(Stack left, Stack right)
{
    return left.amount_ != right.amount_;
}

bool operator<(Stack left, Stack right)
{
    return left.amount_ < right.amount_;
}

bool operator<=(Stack left, Stack right)
{
    return left.amount_ <= right.amount_;
}

bool operator>(Stack left, Stack right)
{
    return left.amount_ > right.amount_;
}

bool operator>=(Stack left, Stack right)
{
    return left.amount_ >= right.amount_;
}
}