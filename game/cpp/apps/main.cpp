#include <iostream>
#include "Deck52.h"


int main(){
    game52::Deck52 deck{};
    deck.shuffle();
    std::cout << deck.to_string();
    return 0;
}

