#include <catch2/catch.hpp>

#include "Hand.h"
#include <vector>

using namespace game52;

namespace {

TEST_CASE("Hands", "[game]") {
    SECTION("HandRanks"){ 
            CHECK(ClassifiedHand::fromString("Kh Td 2c 3s 5d").handRank_ == ClassifiedHand::HighCard);
            CHECK(ClassifiedHand::fromString("Ah Td 2c 3s 5d").handRank_ == ClassifiedHand::HighCard);
            CHECK(ClassifiedHand::fromString("Ah 2h 2c 3s 5d").handRank_ == ClassifiedHand::Pair);
            CHECK(ClassifiedHand::fromString("Ah 2h 2c 4s 5d").handRank_ == ClassifiedHand::Pair);
            CHECK(ClassifiedHand::fromString("Ah Td Tc 3s 5d").handRank_ == ClassifiedHand::Pair);
            CHECK(ClassifiedHand::fromString("Ah Ad 2c 3s 5d").handRank_ == ClassifiedHand::Pair);
            CHECK(ClassifiedHand::fromString("Qh Qd 2c 2s 5d").handRank_ == ClassifiedHand::TwoPair);
            CHECK(ClassifiedHand::fromString("Qh Qd Jc Js 6d").handRank_ == ClassifiedHand::TwoPair);
            CHECK(ClassifiedHand::fromString("Ah Ad 2c 2s 5d").handRank_ == ClassifiedHand::TwoPair);
            CHECK(ClassifiedHand::fromString("Ah 2d 2c 2s 5d").handRank_ == ClassifiedHand::Trips);
            CHECK(ClassifiedHand::fromString("Ah Ad Ac 2s 5d").handRank_ == ClassifiedHand::Trips);
            CHECK(ClassifiedHand::fromString("Ah Ad Ac 3s 5d").handRank_ == ClassifiedHand::Trips);
            CHECK(ClassifiedHand::fromString("Ah 2d 3c 4s 5d").handRank_ == ClassifiedHand::Straight);
            CHECK(ClassifiedHand::fromString("2d 3c 4s 5d 6c").handRank_ == ClassifiedHand::Straight);
            CHECK(ClassifiedHand::fromString("Ah 2h Kh 4h 5h").handRank_ == ClassifiedHand::Flush);
            CHECK(ClassifiedHand::fromString("Ah 2h Kh 6h 5h").handRank_ == ClassifiedHand::Flush);
            CHECK(ClassifiedHand::fromString("5h 2d 2c 2s 5d").handRank_ == ClassifiedHand::FullHouse);
            CHECK(ClassifiedHand::fromString("6h 2d 2c 2s 6d").handRank_ == ClassifiedHand::FullHouse);
            CHECK(ClassifiedHand::fromString("5h 3d 3c 3s 5d").handRank_ == ClassifiedHand::FullHouse);
            CHECK(ClassifiedHand::fromString("5h 2d 2c 2s 2h").handRank_ == ClassifiedHand::Quads);
            CHECK(ClassifiedHand::fromString("5h 3d 3c 3s 3h").handRank_ == ClassifiedHand::Quads);
            CHECK(ClassifiedHand::fromString("Ah 2h 3h 4h 5h").handRank_ == ClassifiedHand::StraightFlush);
            CHECK(ClassifiedHand::fromString("2h 3h 4h 5h 6h").handRank_ == ClassifiedHand::StraightFlush);
            CHECK(ClassifiedHand::fromString("Th Jh Qh Kh Ah").handRank_ == ClassifiedHand::StraightFlush);
    }
    SECTION("HandsSorted") {
        std::vector<ClassifiedHand> hands{
            ClassifiedHand::fromString("Kh Td 2c 3s 5d"),
            ClassifiedHand::fromString("Ah Td 2c 3s 5d"),
            ClassifiedHand::fromString("Ah 2h 2c 3s 5d"),
            ClassifiedHand::fromString("Ah 2h 2c Ks 5d"),
            ClassifiedHand::fromString("Ah 2h 2c Ks Qd"),
            ClassifiedHand::fromString("Ah Td Tc 3s 5d"),
            ClassifiedHand::fromString("Ah Ad 2c 3s 5d"),
            ClassifiedHand::fromString("Qh Qd 2c 2s 5d"),
            ClassifiedHand::fromString("Qh Qd Jc Js 6d"),
            ClassifiedHand::fromString("Ah Ad 2c 2s 5d"),
            ClassifiedHand::fromString("Ah 2d 2c 2s 5d"),
            ClassifiedHand::fromString("Ah Ad Ac 2s 5d"),
            ClassifiedHand::fromString("Ah Ad Ac 3s 5d"),
            ClassifiedHand::fromString("Ah 2d 3c 4s 5d"),
            ClassifiedHand::fromString("2d 3c 4s 5d 6c"),
            ClassifiedHand::fromString("Ah 2h Kh 4h 5h"),
            ClassifiedHand::fromString("Ah 2h Kh 6h 5h"),
            ClassifiedHand::fromString("5h 2d 2c 2s 5d"),
            ClassifiedHand::fromString("6h 2d 2c 2s 6d"),
            ClassifiedHand::fromString("5h 3d 3c 3s 5d"),
            ClassifiedHand::fromString("5h 2d 2c 2s 2h"),
            ClassifiedHand::fromString("5h 3d 3c 3s 3h"),
            ClassifiedHand::fromString("Ah 2h 3h 4h 5h"),
            ClassifiedHand::fromString("2h 3h 4h 5h 6h"),
            ClassifiedHand::fromString("Th Jh Qh Kh Ah"),
        };
        CHECK(std::is_sorted(hands.begin(), hands.end()));
    }
    SECTION("Equality"){
        CHECK(ClassifiedHand::fromString("Ah Kc 2h 7s Td") == ClassifiedHand::fromString("Ac Kc 2h 7d Ts"));
        CHECK(ClassifiedHand::fromString("Ah 2c 2h 7s Td") == ClassifiedHand::fromString("Ac 2s 2h 7d Ts"));
        CHECK(ClassifiedHand::fromString("Ah 2c 2h 7s 7d") == ClassifiedHand::fromString("Ac 2c 2h 7d 7c"));
        CHECK(ClassifiedHand::fromString("Ah Kc 2h 2s 2d") == ClassifiedHand::fromString("Ah Kh 2h 2d 2s"));
        CHECK(ClassifiedHand::fromString("Ah 2c 3h 4s 5d") == ClassifiedHand::fromString("Ac 2c 3h 4d 5s"));
        CHECK(ClassifiedHand::fromString("Ah Kh 3h 4h 5h") == ClassifiedHand::fromString("As Ks 3s 4s 5s"));
        CHECK(ClassifiedHand::fromString("2h 2c 2d 4s 4d") == ClassifiedHand::fromString("2s 2c 2h 4c 4s"));
        CHECK(ClassifiedHand::fromString("2h 2c 2d 2s 4d") == ClassifiedHand::fromString("2s 2c 2h 2d 4s"));
        CHECK(ClassifiedHand::fromString("Ah 2h 3h 4h 5h") == ClassifiedHand::fromString("As 2s 3s 4s 5s"));
        CHECK(ClassifiedHand::fromString("2h 3h 4h 5h 6h") == ClassifiedHand::fromString("2s 3s 4s 5s 6s"));
        
        
                
    }
}

}

