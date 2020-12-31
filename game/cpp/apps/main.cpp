#include <iostream>
#include "Nlhe.h"


int main(){
    game52::Nlhe52 holdem(3);
    for(int i = 0; i < 9; ++i)
        holdem.playHand();
    return 0;
}

