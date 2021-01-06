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
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        std::vector<BettingAction> bets = {
            BettingAction::create(players[0], Preflop, Stack(1000), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(1000), Stack(0), Stack(1000), Decision::Call),
            BettingAction::create(players[2], Preflop, Stack(1500), Stack(0), Stack(1000), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(1500), Stack(1000), Stack(1500), Decision::Call),
        };
        TestDealer dealer(playersInHand, Stack(20));
        dealer.acceptBet(players[0], bets[0]);
        dealer.acceptBet(players[1], bets[1]);
        dealer.acceptBet(players[2], bets[2]);
        dealer.acceptBet(players[1], bets[3]);
        dealer.rakeIn();
        auto sidepots = dealer.getClosedPots();

        CHECK(sidepots.size() == 2);
        CHECK(sidepots[0].get() == Stack(3000));
        CHECK(sidepots[1].get() == Stack(1000));
        CHECK(dealer.getCurrentPot().get() == Stack(0));
        CHECK(playersInHand[0].get().getStack() == Stack(0));
        CHECK(playersInHand[1].get().getStack() == Stack(0));
        CHECK(playersInHand[2].get().getStack() == Stack(500));
    }
    SECTION("Preflop All-In Refund"){
        std::vector<Player> players = {
            Player(Stack(3000), {}, 1),
            Player(Stack(1500), {}, 2),
            Player(Stack(2000), {}, 3),
        };
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        std::vector<BettingAction> bets = {
            BettingAction::create(players[0], Preflop, Stack(2000), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(1500), Stack(0), Stack(2000), Decision::Call),
            BettingAction::create(players[2], Preflop, Stack(0), Stack(0), Stack(2000), Decision::Fold),
        };
        TestDealer dealer(playersInHand, Stack(20));
        dealer.acceptBet(players[0], bets[0]);
        dealer.acceptBet(players[1], bets[1]);
        dealer.acceptBet(players[2], bets[2]);
        dealer.rakeIn();
        auto sidepots = dealer.getClosedPots();

        CHECK(sidepots.size() == 1);
        CHECK(sidepots[0].get() == Stack(3000));
        CHECK(dealer.getCurrentPot().get() == Stack(500));
        CHECK(playersInHand[0].get().getStack() == Stack(1000));
        CHECK(playersInHand[1].get().getStack() == Stack(0));
        CHECK(playersInHand[2].get().getStack() == Stack(2000));
    }
    SECTION("Options normal all-in"){
        std::vector<Player> players = {
            Player(Stack(2500), {}, 1),
            Player(Stack(1500), {}, 2),
            Player(Stack(2000), {}, 3),
        };
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        Deck52 deck;
        deck.shuffle();
        for(auto& player : playersInHand)
            player.get().dealHoleCards(deck.getHoleCards());
        std::vector<BettingAction> preflopBets = {            
            BettingAction::create(players[1], Preflop, Stack(10), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[2], Preflop, Stack(20), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(60), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(0), Stack(10), Stack(60), Decision::Fold),
            BettingAction::create(players[2], Preflop, Stack(200), Stack(20), Stack(60), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(200), Stack(60), Stack(200), Decision::Call),
        };
        TestDealer dealer(playersInHand, Stack(20));

        dealer.acceptBet(players[1], preflopBets[0]);
        dealer.acceptBet(players[2], preflopBets[1]);
        Options options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(20), Stack(20)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(40), Stack(2000)}});
        
        dealer.acceptBet(players[0], preflopBets[2]);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(60), Stack(60)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(100), Stack(1500)}});        
        
        dealer.acceptBet(players[1], preflopBets[3]);
        playersInHand[1].get().dealHoleCards({});
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(60), Stack(60)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(100), Stack(2000)}});
        
        dealer.acceptBet(players[2], preflopBets[4]);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(200), Stack(200)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(340), Stack(2000)}});
        
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
        CHECK(options.options[0] == std::pair{Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Raise, std::pair{Stack(20), Stack(1800)}});
        
        dealer.acceptBet(players[2], flopBets[0]);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(300), Stack(300)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(600), Stack(1800)}});

        dealer.acceptBet(players[0], flopBets[1]);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);

        dealer.rakeIn();

        std::vector<BettingAction> turnBets= {
            BettingAction::create(players[2], Flop, Stack(1500), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Flop, Stack(1500), Stack(0), Stack(1500), Decision::Call),
        };

        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Raise, std::pair{Stack(20), Stack(1500)}});
        
        dealer.acceptBet(players[2], turnBets[0]);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(1500), Stack(1500)}});

        dealer.acceptBet(players[0], turnBets[1]);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);

        dealer.rakeIn();
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);   
    }
    SECTION("Options forced all-in"){
        std::vector<Player> players = {
            Player(Stack(2500), {}, 1),
            Player(Stack(2000), {}, 2),
            Player(Stack(15), {}, 3),
        };
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        Deck52 deck;
        deck.shuffle();
        for(auto& player : playersInHand)
            player.get().dealHoleCards(deck.getHoleCards());
        std::vector<BettingAction> preflopBets = {            
            BettingAction::create(players[1], Preflop, Stack(10), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[2], Preflop, Stack(20), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(60), Stack(0), Stack(20), Decision::Raise),
            BettingAction::create(players[1], Preflop, Stack(60), Stack(10), Stack(60), Decision::Call),
        };
        TestDealer dealer(playersInHand, Stack(20));

        dealer.acceptBet(players[1], preflopBets[0]);
        dealer.acceptBet(players[2], preflopBets[1]);
        Options options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(20), Stack(20)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(40), Stack(2000)}});
        
        dealer.acceptBet(players[0], preflopBets[2]);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(60), Stack(60)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(100), Stack(2000)}});        
        
        dealer.acceptBet(players[1], preflopBets[3]);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 0);
        
        dealer.rakeIn();

        std::vector<BettingAction> flopBets= {
            BettingAction::create(players[1], Flop, Stack(300), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Flop, Stack(300), Stack(0), Stack(300), Decision::Call),
        };

        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Raise, std::pair{Stack(20), Stack(1940)}});


        dealer.acceptBet(players[1], flopBets[0]);
        
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(300), Stack(300)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(600), Stack(1940)}});

        dealer.acceptBet(players[0], flopBets[1]);
        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);

        dealer.rakeIn();

        std::vector<BettingAction> turnBets= {
            BettingAction::create(players[1], Turn, Stack(0), Stack(0), Stack(0), Decision::Check),
            BettingAction::create(players[0], Turn, Stack(1500), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[1], Turn, Stack(1640), Stack(0), Stack(1500), Decision::Raise),
            BettingAction::create(players[0], Turn, Stack(140), Stack(1500), Stack(1640), Decision::Raise),
        };

        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Raise, std::pair{Stack(20), Stack(1640)}});
        
        dealer.acceptBet(players[1], turnBets[0]);

        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);

        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Raise, std::pair{Stack(20), Stack(1640)}});

        dealer.acceptBet(players[0], turnBets[1]);

        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 3);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(1500), Stack(1500)}});
        CHECK(options.options[2] == std::pair{Decision::Raise, std::pair{Stack(1640), Stack(1640)}});

        dealer.acceptBet(players[1], turnBets[2]);

        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);

        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{Decision::Fold, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{Decision::Call, std::pair{Stack(1640), Stack(1640)}});
        
        dealer.acceptBet(players[0], turnBets[3]);
        dealer.rakeIn();

        options = dealer.getOptions(players[1]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
        options = dealer.getOptions(players[0]);
        CHECK(options.options.size() == 0);   
    }
    SECTION("Big Blind Action preflop"){
        std::vector<Player> players = {
            Player(Stack(2500), {}, 1),
            Player(Stack(2000), {}, 2),
            Player(Stack(1500), {}, 3),
        };
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        Deck52 deck;
        deck.shuffle();
        for(auto& player : playersInHand)
            player.get().dealHoleCards(deck.getHoleCards());
        std::vector<BettingAction> preflopBets = {            
            BettingAction::create(players[1], Preflop, Stack(10), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[2], Preflop, Stack(20), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(20), Stack(0), Stack(20), Decision::Call),
            BettingAction::create(players[1], Preflop, Stack(20), Stack(10), Stack(20), Decision::Call),
            BettingAction::create(players[2], Preflop, Stack(0), Stack(0), Stack(0), Decision::Check),
        };
        TestDealer dealer(playersInHand, Stack(20));
        dealer.acceptBet(players[1], preflopBets[0]);
        dealer.acceptBet(players[2], preflopBets[1]);
        dealer.acceptBet(players[0], preflopBets[2]);
        dealer.acceptBet(players[1], preflopBets[3]);
        Options options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 2);
        CHECK(options.options[0] == std::pair{ Decision::Check, std::pair{Stack(0), Stack(0)}});
        CHECK(options.options[1] == std::pair{ Decision::Raise, std::pair{Stack(20), Stack(1500)}});
    }
    SECTION("folded to Big Blind preflop"){
        std::vector<Player> players = {
            Player(Stack(2500), {}, 1),
            Player(Stack(2000), {}, 2),
            Player(Stack(1500), {}, 3),
        };
        std::vector<std::reference_wrapper<Player>> playersInHand;
        for(auto& player : players)
            playersInHand.push_back(player);
        Deck52 deck;
        deck.shuffle();
        for(auto& player : playersInHand)
            player.get().dealHoleCards(deck.getHoleCards());
        std::vector<BettingAction> preflopBets = {            
            BettingAction::create(players[1], Preflop, Stack(10), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[2], Preflop, Stack(20), Stack(0), Stack(0), Decision::Raise),
            BettingAction::create(players[0], Preflop, Stack(0), Stack(0), Stack(20), Decision::Fold),
            BettingAction::create(players[1], Preflop, Stack(0), Stack(10), Stack(20), Decision::Fold),
        };
        TestDealer dealer(playersInHand, Stack(20));
        dealer.acceptBet(players[1], preflopBets[0]);
        dealer.acceptBet(players[2], preflopBets[1]);
        dealer.acceptBet(players[0], preflopBets[2]);
        players[0].dealHoleCards({});
        dealer.acceptBet(players[1], preflopBets[3]);
        players[1].dealHoleCards({});
        Options options = dealer.getOptions(players[2]);
        CHECK(options.options.size() == 0);
    }
}

}