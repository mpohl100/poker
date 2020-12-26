#pragma once

#include <string>

namespace game52{

struct Stack{
    Stack(int amt) : amount_(amt) {};
    Stack() = default;
    Stack(Stack const&) = default;
    Stack& operator=(Stack const&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    Stack getAmount(Stack amt); 
     
    Stack& operator+=(Stack other);
    Stack& operator-=(Stack other);
    std::string toString() const;
     
    int amount_ = 0;
    
    friend auto operator<=>(const Stack&, const Stack&) = default;
};

Stack operator-(Stack left, Stack right);

}