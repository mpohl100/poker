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

Stack& Stack::operator+=(Stack other)
{
    amount_ += other.amount_;
    return *this;
}

Stack& Stack::operator-=(Stack other)
{
    amount_ -= other.amount_;
    return *this;
}

Stack& Stack::operator*=(int mult)
{
    amount_ *= mult;
    return *this;
}

Stack& Stack::operator/=(int div)
{
    amount_ /= div;
    return *this;
}

std::string Stack::toString() const
{
    return std::to_string(amount_);
}

Stack operator-(Stack left, Stack right)
{
    Stack ret = left;
    ret -= right;
    return ret;
}

Stack operator+(Stack left, Stack right)
{
    Stack ret = left;
    ret += right;
    return ret;
}

Stack operator*(Stack left, int mult)
{
    Stack ret = left;
    ret *= mult;
    return ret;
}

Stack operator/(Stack left, int div)
{
    Stack ret = left;
    ret /= div;
    return ret;
}
}