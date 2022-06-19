#include <catch2/catch.hpp>

#include "Hand.h"
#include <vector>

using namespace game52;

namespace {

TEST_CASE("Hands", "[game]") {
    SECTION("HandRanks"){ 
            CHECK(MadeHand52::fromString("Kh Td 2c 3s 5d").handRank_ == MadeHand52::HandRank52::HighCard);
            CHECK(MadeHand52::fromString("Ah Td 2c 3s 5d").handRank_ == MadeHand52::HandRank52::HighCard);
            CHECK(MadeHand52::fromString("Kh 2h 2c Qs Jd").handRank_ == MadeHand52::HandRank52::Pair);
            CHECK(MadeHand52::fromString("Ah 2h 2c 3s 5d").handRank_ == MadeHand52::HandRank52::Pair);
            CHECK(MadeHand52::fromString("Ah 2h 2c 4s 5d").handRank_ == MadeHand52::HandRank52::Pair);
            CHECK(MadeHand52::fromString("Ah Td Tc 3s 5d").handRank_ == MadeHand52::HandRank52::Pair);
            CHECK(MadeHand52::fromString("Ah Ad 2c 3s 5d").handRank_ == MadeHand52::HandRank52::Pair);
            CHECK(MadeHand52::fromString("Qh Qd 2c 2s 5d").handRank_ == MadeHand52::HandRank52::TwoPair);
            CHECK(MadeHand52::fromString("Qh Qd Jc Js 6d").handRank_ == MadeHand52::HandRank52::TwoPair);
            CHECK(MadeHand52::fromString("Ah Ad 2c 2s 5d").handRank_ == MadeHand52::HandRank52::TwoPair);
            CHECK(MadeHand52::fromString("Ah 2d 2c 2s 5d").handRank_ == MadeHand52::HandRank52::Trips);
            CHECK(MadeHand52::fromString("Ah Ad Ac 2s 5d").handRank_ == MadeHand52::HandRank52::Trips);
            CHECK(MadeHand52::fromString("Ah Ad Ac 3s 5d").handRank_ == MadeHand52::HandRank52::Trips);
            CHECK(MadeHand52::fromString("Ah 2d 3c 4s 5d").handRank_ == MadeHand52::HandRank52::Straight);
            CHECK(MadeHand52::fromString("2d 3c 4s 5d 6c").handRank_ == MadeHand52::HandRank52::Straight);
            CHECK(MadeHand52::fromString("Ah 2h Kh 4h 5h").handRank_ == MadeHand52::HandRank52::Flush);
            CHECK(MadeHand52::fromString("Ah 2h Kh 6h 5h").handRank_ == MadeHand52::HandRank52::Flush);
            CHECK(MadeHand52::fromString("5h 2d 2c 2s 5d").handRank_ == MadeHand52::HandRank52::FullHouse);
            CHECK(MadeHand52::fromString("6h 2d 2c 2s 6d").handRank_ == MadeHand52::HandRank52::FullHouse);
            CHECK(MadeHand52::fromString("5h 3d 3c 3s 5d").handRank_ == MadeHand52::HandRank52::FullHouse);
            CHECK(MadeHand52::fromString("5h 2d 2c 2s 2h").handRank_ == MadeHand52::HandRank52::Quads);
            CHECK(MadeHand52::fromString("5h 3d 3c 3s 3h").handRank_ == MadeHand52::HandRank52::Quads);
            CHECK(MadeHand52::fromString("Ah 2h 3h 4h 5h").handRank_ == MadeHand52::HandRank52::StraightFlush);
            CHECK(MadeHand52::fromString("2h 3h 4h 5h 6h").handRank_ == MadeHand52::HandRank52::StraightFlush);
            CHECK(MadeHand52::fromString("Th Jh Qh Kh Ah").handRank_ == MadeHand52::HandRank52::StraightFlush);
    }
    SECTION("HandsSorted") {
        std::vector<MadeHand52> hands{
            MadeHand52::fromString("Kh Td 2c 3s 5d"),
            MadeHand52::fromString("Ah Td 2c 3s 5d"),
            MadeHand52::fromString("Kh 2h 2c Qs Jd"),
            MadeHand52::fromString("Ah 2h 2c 3s 5d"),
            MadeHand52::fromString("Ah 2h 2c Ks 5d"),
            MadeHand52::fromString("Ah 2h 2c Ks Qd"),
            MadeHand52::fromString("Ah Td Tc 3s 5d"),
            MadeHand52::fromString("Ah Ad 2c 3s 5d"),
            MadeHand52::fromString("Qh Qd 2c 2s 5d"),
            MadeHand52::fromString("Qh Qd Jc Js 6d"),
            MadeHand52::fromString("Ah Ad 2c 2s 5d"),
            MadeHand52::fromString("Ah 2d 2c 2s 5d"),
            MadeHand52::fromString("Ah Ad Ac 2s 5d"),
            MadeHand52::fromString("Ah Ad Ac 3s 5d"),
            MadeHand52::fromString("Ah 2d 3c 4s 5d"),
            MadeHand52::fromString("2d 3c 4s 5d 6c"),
            MadeHand52::fromString("Ah 2h Kh 4h 5h"),
            MadeHand52::fromString("Ah 2h Kh 6h 5h"),
            MadeHand52::fromString("5h 2d 2c 2s 5d"),
            MadeHand52::fromString("6h 2d 2c 2s 6d"),
            MadeHand52::fromString("5h 3d 3c 3s 5d"),
            MadeHand52::fromString("5h 2d 2c 2s 2h"),
            MadeHand52::fromString("5h 3d 3c 3s 3h"),
            MadeHand52::fromString("Ah 2h 3h 4h 5h"),
            MadeHand52::fromString("2h 3h 4h 5h 6h"),
            MadeHand52::fromString("Th Jh Qh Kh Ah"),
        };
        CHECK(std::is_sorted(hands.begin(), hands.end()));
    }
    SECTION("Equality"){
        CHECK(MadeHand52::fromString("Ah Kc 2h 7s Td") == MadeHand52::fromString("Ac Kc 2h 7d Ts"));
        CHECK(MadeHand52::fromString("Ah 2c 2h 7s Td") == MadeHand52::fromString("Ac 2s 2h 7d Ts"));
        CHECK(MadeHand52::fromString("Ah 2c 2h 7s 7d") == MadeHand52::fromString("Ac 2c 2h 7d 7c"));
        CHECK(MadeHand52::fromString("Ah Kc 2h 2s 2d") == MadeHand52::fromString("Ah Kh 2h 2d 2s"));
        CHECK(MadeHand52::fromString("Ah 2c 3h 4s 5d") == MadeHand52::fromString("Ac 2c 3h 4d 5s"));
        CHECK(MadeHand52::fromString("Ah Kh 3h 4h 5h") == MadeHand52::fromString("As Ks 3s 4s 5s"));
        CHECK(MadeHand52::fromString("2h 2c 2d 4s 4d") == MadeHand52::fromString("2s 2c 2h 4c 4s"));
        CHECK(MadeHand52::fromString("2h 2c 2d 2s 4d") == MadeHand52::fromString("2s 2c 2h 2d 4s"));
        CHECK(MadeHand52::fromString("Ah 2h 3h 4h 5h") == MadeHand52::fromString("As 2s 3s 4s 5s"));
        CHECK(MadeHand52::fromString("2h 3h 4h 5h 6h") == MadeHand52::fromString("2s 3s 4s 5s 6s"));           
    }


SECTION("DrawRanks"){ 
            CHECK(DrawingHand52::fromString("Kh Td 2c 3s").handRank_ == DrawingHand52::DrawRank52::None);
            CHECK(DrawingHand52::fromString("Ah 2d 3c 4s").handRank_ == DrawingHand52::DrawRank52::WheelGutshot);
            CHECK(DrawingHand52::fromString("2d 3c 4s 6d").handRank_ == DrawingHand52::DrawRank52::Gutshot);
            CHECK(DrawingHand52::fromString("Jd Qc Ks Ad").handRank_ == DrawingHand52::DrawRank52::Gutshot);
            CHECK(DrawingHand52::fromString("2d 3c 4s 5d").handRank_ == DrawingHand52::DrawRank52::Openend);
            CHECK(DrawingHand52::fromString("Ah 2h Kh 4h").handRank_ == DrawingHand52::DrawRank52::Flush);
            CHECK(DrawingHand52::fromString("Ah 2h Kh 6h").handRank_ == DrawingHand52::DrawRank52::Flush);
            CHECK(DrawingHand52::fromString("Ah 2h 3h 4h").handRank_ == DrawingHand52::DrawRank52::FlushWheelGutshot);
            CHECK(DrawingHand52::fromString("2h 3h 4h 6h").handRank_ == DrawingHand52::DrawRank52::FlushGutshot);
            CHECK(DrawingHand52::fromString("Jh Qh Kh Ah").handRank_ == DrawingHand52::DrawRank52::FlushGutshot);
            CHECK(DrawingHand52::fromString("2h 3h 4h 5h").handRank_ == DrawingHand52::DrawRank52::FlushOpenend);
            CHECK(DrawingHand52::fromString("Th Jh Qh Kh").handRank_ == DrawingHand52::DrawRank52::FlushOpenend);
    }
    SECTION("DrawsSorted") {
        std::vector<DrawingHand52> hands{
            DrawingHand52::fromString("Kh Td 2c 3s"),
            DrawingHand52::fromString("Ah 2d 3c 4s"),
            DrawingHand52::fromString("2h 3d 4c 6s"),
            DrawingHand52::fromString("Jd Qc Ks Ad"),
            DrawingHand52::fromString("2d 3c 4s 5d"),
            DrawingHand52::fromString("Ah 2h Kh 4h"),
            DrawingHand52::fromString("Ah 2h Kh 6h"),
            DrawingHand52::fromString("Ah 2h 3h 4h"),
            DrawingHand52::fromString("Jh Qh Kh Ah"),
            DrawingHand52::fromString("2h 3h 4h 5h"),
            DrawingHand52::fromString("Th Jh Qh Kh"),
        };
        CHECK(std::is_sorted(hands.begin(), hands.end()));
    }
    SECTION("DrawEquality"){
        CHECK(DrawingHand52::fromString("Ah Kc 2h 7s") == DrawingHand52::fromString("Ac Kc 2h 7d"));
        CHECK(DrawingHand52::fromString("Ah 2c 3h 4s") == DrawingHand52::fromString("Ac 2c 3h 4d"));
        CHECK(DrawingHand52::fromString("2c 3h 4s 6h") == DrawingHand52::fromString("2c 3h 4d 6c"));
        CHECK(DrawingHand52::fromString("Ah Kh 3h 4h") == DrawingHand52::fromString("As Ks 3s 4s"));
        CHECK(DrawingHand52::fromString("Ah 2h 3h 4h") == DrawingHand52::fromString("As 2s 3s 4s"));
        CHECK(DrawingHand52::fromString("2h 3h 4h 5h") == DrawingHand52::fromString("2s 3s 4s 5s"));           
    }



}

}

