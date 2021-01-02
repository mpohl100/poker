#include <catch2/catch.hpp>

#include "Dealer.h"
#include "Action.h"
#include "Deck52.h"
#include "DecisionEngine.h"

using namespace game52;

namespace{

class TestDealer : public Dealer{
public:
    using Dealer::Dealer;
    std::vector<Pot> const& getClosedPots()
    {
        return closedPots_;
    }
};

TEST_CASE("Dealer", "[game]"){
    SECTION("Preflop All-In"){
        std::vector<Player> players = {
            Player(Stack(1000), {}, 1),
            Player(Stack(1500), {}, 2),
            Player(Stack(2000), {}, 3),
        };
        std::vector<BettingAction> bets = {
            BettingAction::create(players[0], Preflop, Stack(1000), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(1000), Stack(0), Stack(1000), Decision::Call),
            BettingAction::create(players[2], Preflop, Stack(2000), Stack(0), Stack(1000), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(1500), Stack(1000), Stack(2000), Decision::Call),
        };
        TestDealer dealer(Stack(20));
        dealer.acceptBet(players[0], bets[0]);
        dealer.acceptBet(players[1], bets[1]);
        dealer.acceptBet(players[2], bets[2]);
        dealer.acceptBet(players[1], bets[3]);
        dealer.rakeIn();
        auto sidepots = dealer.getClosedPots();

        CHECK(sidepots[0].get() == Stack(3000));
        CHECK(sidepots[1].get() == Stack(1000));
        CHECK(sidepots[2].get() == Stack(500));
        CHECK(dealer.getCurrentPot().get() == Stack(0));
        CHECK(players[0].getStack() == Stack(0));
        CHECK(players[1].getStack() == Stack(0));
        CHECK(players[2].getStack() == Stack(0));
    }
    SECTION("Options"){
        std::vector<Player> players = {
            Player(Stack(1000), {}, 1),
            Player(Stack(1500), {}, 2),
            Player(Stack(2000), {}, 3),
        };
        Deck52 deck;
        deck.shuffle();
        for(auto& player : players) {
            player.dealHoleCards(deck.getHoleCards());
        }
        std::vector<BettingAction> preflopBets = {            
            BettingAction::create(players[1], Preflop, Stack(10), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[2], Preflop, Stack(20), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(60), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(0), Stack(10), Stack(60), Decision::Fold),
            BettingAction::create(players[2], Preflop, Stack(200), Stack(20), Stack(60), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(200), Stack(60), Stack(200), Decision::Call),
        };
        TestDealer dealer(Stack(20));

        dealer.acceptBet(players[1], preflopBets[0]);
        dealer.acceptBet(players[2], preflopBets[1]);
        Options options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Call, Stack(20)});
        CHECK(options.options[2] == std::pair{Decision::Raise, Stack(40)});
        
        dealer.acceptBet(players[0], preflopBets[2]);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Call, Stack(60)});
        CHECK(options.options[2] == std::pair{Decision::Raise, Stack(100)});        
        
        dealer.acceptBet(players[1], preflopBets[3]);
        players[1].dealHoleCards({});
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Call, Stack(60)});
        CHECK(options.options[2] == std::pair{Decision::Raise, Stack(100)});
        
        dealer.acceptBet(players[2], preflopBets[4]);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Call, Stack(200)});
        CHECK(options.options[2] == std::pair{Decision::Raise, Stack(340)});
        
        dealer.acceptBet(players[0], preflopBets[5]);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);

        dealer.rakeIn();

        std::vector<BettingAction> flopBets= {
            BettingAction::create(players[2], Flop, Stack(300), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Flop, Stack(300), Stack(0), Stack(300), Decision::Call),
        };

        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Check, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Raise, Stack(20)});
        
        dealer.acceptBet(players[2], flopBets[0]);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, Stack(0)});
        CHECK(options.options[1] == std::pair{Decision::Call, Stack(300)});
        CHECK(options.options[2] == std::pair{Decision::Raise, Stack(600)});

        dealer.acceptBet(players[0], flopBets[1]);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);
    }
}

}