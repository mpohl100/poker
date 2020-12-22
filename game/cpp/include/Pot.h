#pragma once

namespace game52{

class Pot{
    public:
        void put_amount(int amt);
    private:
        int amount_ = 0;
};

}