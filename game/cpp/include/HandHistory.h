#pragma once

#include "Action.h"

#include <vector>
#include <optional>

namespace game52{

class HandHistory{
    public:
        void logAction(std::unique_ptr<HandAction> action);
        std::optional<BettingAction> getLastBet(Street street, const Player* player = nullptr) const;
        std::string toString() const;
    private:
        std::vector<std::unique_ptr<HandAction>> actions_;
};

}