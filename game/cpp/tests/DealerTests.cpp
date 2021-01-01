#include <catch2/catch.hpp>

#include "Dealer.h"
#include "Action.h"
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
        TestDealer dealer;
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
}

}