#include "Board.h"
#include "Hand.h"
#include "HoleCards.h"

#include <range/v3/all.hpp>

namespace game52{

Street Board::street() const
{
    switch(cards_.size()){
        case 0: return Preflop;
        case 3: return Flop;
        case 4: return Turn;
        case 5: return River;
        default: break;
    }
    return Preflop;
}

std::string toString(Street street){
    switch (street)
    {
        case Preflop: return "*** HOLE CARDS ***";
        case Flop: return "*** FLOP ***";
        case Turn: return "*** TURN ***";
        case River: return "*** RIVER ***";
    };
    return "Unknown Cards";
}

void Board::dealFlop(std::vector<Card52> const& flop)
{
    cards_ = flop;
}

void Board::dealTurn(Card52 turn)
{
    cards_.push_back(turn);
}

void Board::dealRiver(Card52 river)
{
    cards_.push_back(river);
}


std::vector<Card52> const& Board::getCards() const
{
    return cards_;
}

std::vector<HoleCards> Board::getBestHoldings() const
{
    auto nbGen = [i = 0]() mutable { return Card52(i++); };
    auto cards = ranges::views::generate(nbGen) | ranges::views::take(52) | ranges::to<std::vector>;
    auto sortedHoldings = ranges::views::cartesian_product(cards, cards)
                     | ranges::views::filter([](std::tuple<Card52,Card52> const& holeCards){ return std::get<0>(holeCards) != std::get<1>(holeCards);})
                     | ranges::views::transform([this](std::tuple<Card52, Card52> const& holeCards){ 
                         HoleCards holding = HoleCards(std::get<0>(holeCards), std::get<1>(holeCards));
                         return std::pair{ holding, getBestHand(Hand(holding, *this)) }; 
                         })
                     | ranges::to<std::vector>
                     | ranges::actions::sort([](const auto& l, const auto&r){ return l.second > r.second; });
    std::vector<HoleCards> ret;
    for(const auto& pr : sortedHoldings)
        ret.push_back(pr.first);
    return ret;
}

}