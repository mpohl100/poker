#include "Nlhe.h"
#include "HandRange.h"

#include <clara.hpp>

#include <iostream>


int main(int argc, char** argv){
    using namespace clara;


    int nbHands = 100;
    bool help = false;
    auto cli = Opt(nbHands, "numberHands")["-n"]["--nb_hands"]("number of hands to play") | Help(help);
     

    auto result = cli.parse(Args(argc, argv));
    if (!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << '\n';
        exit(1);
    }
    if (help) {
        std::cout << cli;
        exit(0);
    }

    game52::calculatePreflopStrengths();

    //game52::Nlhe52 holdem(3);
    //for(int i = 0; i < nbHands; ++i)
    //    holdem.playHand();
    return 0;
}

