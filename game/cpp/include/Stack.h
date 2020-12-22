#pragma once

namespace game52{

struct Stack{
    Stack(int amt) : amount_(amt) {};
    Stack() = default;
    Stack(Stack const&) = default;
    Stack& operator=(Stack const&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    Stack getAmount(Stack amt); 
     
    Stack& operator+=(Stack const& other);
     
    int amount_ = 0;
};

bool operator==(Stack left, Stack right);
bool operator!=(Stack left, Stack right);

bool operator<(Stack left, Stack right);
bool operator<=(Stack left, Stack right);
bool operator>(Stack left, Stack right);
bool operator>=(Stack left, Stack right);

}