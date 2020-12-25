#pragma once

#include "Action.h"

#include <vector>

namespace game52{

class HandHistory{
    public:
        void logAction(HandAction const& action);
        HandAction getLast();
        std::string toString() const;
    private:
        std::vector<HandAction> actions_;


};

}