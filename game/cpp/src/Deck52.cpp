#include "Deck52.h"

namespace game52{

Deck52::Deck52()
{
    for(int i = 0; i < 52; ++i)
        cards_.push_back(Card52(i));
}

}