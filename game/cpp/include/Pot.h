#pragma once

#include "Stack.h"

namespace game52{

class Pot{
    public:
        void putAmount(Stack amt);
        int getAmount() const;
        Stack get();
        std::string toString() const;
    private:
        Stack amount_ = 0;
};

}