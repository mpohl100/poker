#include "HandRange.h"
#include "Deck52.h"
#include "Hand.h"
#include "RatedHand.h"

#include <range/v3/all.hpp>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <map>

namespace game52{

HoleCards getRepresentativeHolding(HandRangeCont<float>::Key key)
{
    if(key.suited)
        return HoleCards(Card52(key.rank1, Hearts), Card52(key.rank2, Hearts));
    return HoleCards(Card52(key.rank1, Hearts), Card52(key.rank2, Diamonds));
}

float deduceHandStrength(MadeHand52 const& madeHand)
{
    return float(madeHand.handRank_);
}

float deduceHandStrength(DrawingHand52 const& drawingHand)
{
    return float(drawingHand.handRank_);
}

float getHandStrength(HandRangeCont<float>::Key key)
{
    float strength = 0;
    HoleCards holeCards = getRepresentativeHolding(key);
    Deck52 deck;
    deck.moveToFront(holeCards.getCards());
    size_t N = 1000;
    for(size_t i = 0; i < N; ++i)
    {
        deck.shuffleRemainder();
        auto rng = deck.getRange(5);
        MadeHand52 madeHand(rng.first, rng.second);
        strength += deduceHandStrength(madeHand);
        DrawingHand52 drawingHand(rng.first, rng.second);
        strength += deduceHandStrength(drawingHand);
    } 
    return strength / N;
}

HandRangeCont<float> getPreflopStrengths()
{
    HandRangeCont<float> strengths;
    for(int i = int(Deuce); i <= Ace; ++i)
        for(int j = i; j <= Ace; ++j)
        {
            Rank52 rank1 = static_cast<Rank52>(i);
            Rank52 rank2 = static_cast<Rank52>(j);
            for( bool suited : std::vector{true, false})
            {
                if( suited and rank1 == rank2)
                    continue;
                auto key = HandRangeCont<float>::Key(rank2, rank1, suited);
                strengths.cont[key] = getHandStrength(key);
            }
        }
    return strengths;
}

void printPreflopStrengths()
{
    HandRangeCont<float> strengths = getPreflopStrengths();
    std::vector<std::pair<HandRangeCont<float>::Key, float>> flattenedStrengths;
    for(const auto& pr : strengths.cont)
        flattenedStrengths.push_back(pr);
    std::sort(flattenedStrengths.begin(), flattenedStrengths.end(), [](const auto& l, const auto& r){ return l.second > r.second; });
    for(const auto& strength : flattenedStrengths)
        std::cout << getRepresentativeHolding(strength.first).toString() << ' ' << strength.second << '\n';
}

void calculatePreflopStrengths()
{
    std::multimap<double, HoleCards> results;
    auto holecards = HoleCards::getAll();
    for(HoleCards const& holding : holecards)
    {
        RatedHand ratedHand(holding, Board{});
        double score = ratedHand.getSimulatedScore();
        results.insert({score, holding});
    }
    for(auto it = results.rbegin(); it != results.rend(); ++it)
        std::cout << "Holding: " << it->second.toString() << ": score " << it->first << '\n'; 
}

}