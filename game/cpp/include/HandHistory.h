#pragma once

#include "Action.h"

#include <vector>

namespace game52{

class HandHistory{
    public:
        void logAction(std::unique_ptr<HandAction> action);
        BettingAction getLastBet(Street street);
        std::string toString() const;
    private:
        std::vector<std::unique_ptr<HandAction>> actions_;
};

}