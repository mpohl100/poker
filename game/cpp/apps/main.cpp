#include <iostream>
#include "Nlhe.h"


int main(){
    game52::Nlhe52 holdem(2);
    std::cout << "playing hand\n";
    holdem.playHand();
    return 0;
}

