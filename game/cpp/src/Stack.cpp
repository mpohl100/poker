#include "Stack.h"

namespace game52{

int Stack::get_amount(int amt){
    if(amt > amount_)
    {
        amount_ = 0;
        return amount_;
    }
    else
    {
        amount_ -= amt;
        return amt;
    }
}    

}