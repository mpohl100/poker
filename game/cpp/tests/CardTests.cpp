#include <catch2/catch.hpp>

#include "Card52.h"
#include <string>

using namespace game52;

namespace {

TEST_CASE("Cards", "[game]") {
    SECTION("CardsFromEnums"){
        Card52 card(Deuce, Hearts);
        CHECK(card.rank() == Deuce);
        CHECK(card.suit() == Hearts);
        Card52 card2(Deuce, Spades);
        CHECK(card2.rank() == Deuce);
        CHECK(card2.suit() == Spades);
    }
    SECTION("CardsFromString"){
        Card52 card("2h");
        CHECK(card.rank() == Deuce);
        CHECK(card.suit() == Hearts);
        Card52 card2("2s");
        CHECK(card2.rank() == Deuce);
        CHECK(card2.suit() == Spades);
    } 
}

}