#pragma once

namespace game52{

struct Stack{
    Stack(int amt) : amount_(amt) {};
    Stack() = default;
    Stack(Stack const&) = default;
    Stack& operator=(Stack const&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;
    int get_amount(int amt); 

    int amount_ = 0;
};



}