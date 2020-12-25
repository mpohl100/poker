#include "HandHistory.h"

namespace game52{

void HandHistory::logAction(HandAction const& handAction)
{
    actions_.push_back(handAction);
}

HandAction HandHistory::getLast()
{
    return *actions_.rbegin();
}


std::string HandHistory::toString() const
{
    std::string str;
    for(const auto& action : actions_)
        str += action.toString() + '\n';
    return str;
}

}