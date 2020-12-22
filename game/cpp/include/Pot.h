#pragma once

#include "Stack.h"

namespace game52{

class Pot{
    public:
        void putAmount(Stack amt);
        Stack get();
    private:
        Stack amount_ = 0;
};

}