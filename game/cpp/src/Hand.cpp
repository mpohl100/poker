#include "Hand.h"
#include "HoleCards.h"
#include "Board.h"
#include "HowardsCombinatorics.h"


#include <range/v3/all.hpp>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <numeric>
#include <array>

namespace game52 {

Hand::Hand(HoleCards const& holeCards, Board const& board)
    : holeCards_(holeCards)
    , board_(board)
{
    cards_ = holeCards.getCards();
    const auto& boardCards = board.getCards();
    cards_.insert(cards_.end(), boardCards.begin(), boardCards.end());
}

std::vector<Card52> Hand::getCards() const
{
    return cards_;
}

HoleCards const& Hand::getHoleCards() const
{
    return holeCards_;
}

std::string Hand::toString() const
{
    MadeHand52 classified = getBestHand(*this);
    switch(classified.handRank_){
        case MadeHand52::HighCard:{
            return "High Card.";
        }break;
        case MadeHand52::Pair:{
            return "a pair.";
        }break;
        case MadeHand52::TwoPair: {
            return "two pair.";
        }break;
        case MadeHand52::Trips:{
            return "three of a kind.";
        }break;
        case MadeHand52::Straight:{
            return "a straight.";
        }break;
        case MadeHand52::Flush:{
            return "a flush.";
        }break;
        case MadeHand52::FullHouse:{
            return "a full house.";
        }break;
        case MadeHand52::Quads:{
            return "four of a kind.";
        }break;
        case MadeHand52::StraightFlush:{
            return "a straight flush.";
        }
        default: return "";
    }
}

MadeHand52::MadeHand52(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end )
    : cards_({begin, end}) 
{
    if(cards_.size() != 5)
        throw std::runtime_error("classified hand != size 5");
    for(auto& card : cards_)
        rankOccurences_[card.rank()]++;
    if(rankOccurences_.size() == 5) // HighCard, Flush, Straight or StraightFlush
    {
        // check for flush
        std::map<Suit, int> suit_occurences;
        for(const auto& card : cards_)
            suit_occurences[card.suit()]++;
        bool isFlush = suit_occurences.size() == 1;

        // check for straight except by wheel (A,2,3,4,5)
        auto compare = [](const auto& l, const auto& r){ return l.rank() < r.rank();};
        auto minRank = *std::min_element(cards_.begin(), cards_.end(), compare);
        auto maxRank = *std::max_element(cards_.begin(), cards_.end(), compare);
        bool isStraight = (maxRank.rank() - minRank.rank()) == 4;

        // check for wheel (A,2,3,4,5)
        auto compareWheel = [](const auto& l, const auto& r){ 
            int leftRank = l.rank();
            if(leftRank == Ace)
                leftRank -= 13;
            int rightRank = r.rank();
            if(rightRank == Ace)
                rightRank -= 13; 
            return leftRank < rightRank;
        };
        minRank = *std::min_element(cards_.begin(), cards_.end(), compareWheel);
        maxRank = *std::max_element(cards_.begin(), cards_.end(), compareWheel);
        bool isWheel = minRank.rank() == Ace and maxRank.rank() == Five;

        // set the handrank
        if(not isWheel and not isStraight and not isFlush)
            handRank_ = HighCard;
        if(isStraight or isWheel)
            handRank_ = Straight;
        if(isFlush)
            handRank_ = Flush;
        if((isStraight or isWheel) and isFlush)
            handRank_ = StraightFlush;
    }
    else // Pair, TwoPair, Trips, FullHouse or Quads
    {
        if(rankOccurences_.size() == 4) // Pair
            handRank_ = Pair;
        else if(rankOccurences_.size() == 3) // Trips or Two Pair
        {
            int maxOccurence = 0;
            for(const auto& [rank, occ] : rankOccurences_)
            {
                if(occ > maxOccurence)
                    maxOccurence = occ;
            }
            if(maxOccurence == 3)
                handRank_ = Trips;
            else
                handRank_ = TwoPair;
        }
        else if(rankOccurences_.size() == 2) // Full House or Quads
        {
            int maxOccurence = 0;
            for(const auto& [rank, occ] : rankOccurences_)
            {
                if(occ > maxOccurence)
                    maxOccurence = occ;
            }
            if(maxOccurence == 4)
                handRank_ = Quads;
            else
                handRank_ = FullHouse;
        
        }
    }
};

std::array<Rank52,2> MadeHand52::findOccurences(int nb) const
{
    std::array<Rank52,2> ret;
    int index = 0;
    for(const auto& [rank, occ] : rankOccurences_)
        if(occ == nb)
            ret[index++] = rank;
    if(index == 2)
        if(ret[0] < ret[1])
            std::swap(ret[0], ret[1]);
    return ret;
};

std::vector<Rank52> MadeHand52::getHighCards() const
{
    auto highCards = rankOccurences_ | ranges::view::filter([](const auto& pr){ return pr.second == 1; })
                                    | ranges::view::transform([](const auto& pr){ return pr.first;})
                                    | ranges::to<std::vector>() 
                                    | ranges::action::sort 
                                    | ranges::action::reverse;
    return highCards;
}

int MadeHand52::sum() const
{
    return std::accumulate(cards_.begin(), cards_.end(), 0, [](int l, const Card52& r){return l + r.rank();});
}

MadeHand52 MadeHand52::fromString(std::string const& str)
{
    auto cards =  str | std::ranges::views::split(' ')
                      | std::ranges::views::transform([](auto &&rng) {
        return std::string(&*rng.begin(), std::ranges::distance(rng));
    });
    std::vector<Card52> hand;
    for(auto card : cards)
        hand.push_back(Card52(card));
    return MadeHand52(hand.begin(), hand.end());
}

bool compareHighCards(std::vector<Rank52> const& l, std::vector<Rank52> const& r)
{
    return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
}

bool operator<(MadeHand52 const& l, MadeHand52 const& r)
{
    if(l.handRank_ != r.handRank_)
        return l.handRank_ < r.handRank_;
    // detailed looking into the higher hand

    // high card
    if(l.handRank_ == MadeHand52::HighCard)
    {
        return compareHighCards(l.getHighCards(), r.getHighCards());
    }
    if(l.handRank_ == MadeHand52::Pair)
    {
        auto lpairRank = l.findOccurences(2);
        auto rPairRank = r.findOccurences(2);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return compareHighCards(l.getHighCards(), r.getHighCards()); // if pairs are even, the higher card wins
    }
    if(l.handRank_ == MadeHand52::TwoPair)
    {
        auto lpairRank = l.findOccurences(2);
        auto rPairRank = r.findOccurences(2); 
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        if(lpairRank[1] != rPairRank[1])
            return lpairRank[1] < rPairRank[1];
        return compareHighCards(l.getHighCards(), r.getHighCards()); // if pairs are even, the higher card wins
    }
    if(l.handRank_ == MadeHand52::Trips)
    {
        auto lpairRank = l.findOccurences(3);
        auto rPairRank = r.findOccurences(3);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return compareHighCards(l.getHighCards(), r.getHighCards()); // if trips are even, the higher card wins
    }
    if(l.handRank_ == MadeHand52::Straight)
    {
        // not easy you have to take wheel into account
        int lSum = l.sum();
        if(lSum == Ace + Deuce + Three + Four + Five)
            lSum -= Ace;
        int rSum = r.sum();
        if(rSum == Ace + Deuce + Three + Four + Five)
            rSum -= Ace;
        return lSum < rSum;
    }
    if(l.handRank_ == MadeHand52::Flush)
    {
        return compareHighCards(l.getHighCards(), r.getHighCards());
    }
    if(l.handRank_ == MadeHand52::FullHouse)
    {
        // check trips
        auto lpairRank = l.findOccurences(3);
        auto rPairRank = r.findOccurences(3);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        // check pair
        lpairRank = l.findOccurences(2);
        rPairRank = r.findOccurences(2);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return compareHighCards(l.getHighCards(), r.getHighCards());
    }
    if(l.handRank_ == MadeHand52::Quads)
    {
        auto lpairRank = l.findOccurences(4);
        auto rPairRank = r.findOccurences(4);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return compareHighCards(l.getHighCards(), r.getHighCards());
    }
    if( l.handRank_ == MadeHand52::StraightFlush)
    {
        int lSum = l.sum();
        if(lSum == Ace + Deuce + Three + Four + Five)
            lSum -= Ace;
        int rSum = r.sum();
        if(rSum == Ace + Deuce + Three + Four + Five)
            rSum -= Ace;
        return lSum < rSum;
    }
    // we shouldn't come here
    throw std::runtime_error("programming error: reached end of classified hand operator<.");
    return false;
}

bool operator<=(MadeHand52 const& l, MadeHand52 const& r)
{
    return not(l>r);
}

bool operator>(MadeHand52 const& l, MadeHand52 const& r)
{
    return r < l;
}

bool operator>=(MadeHand52 const& l, MadeHand52 const& r)
{
    return not(l<r);
}

bool operator==(MadeHand52 const& l, MadeHand52 const& r)
{
    return not (l<r) and not (r<l);
}


bool operator!=(MadeHand52 const& l, MadeHand52 const& r)
{
    return not (l == r);
}

MadeHand52 getBestHand(Hand hand)
{ 
    auto cards = hand.getCards();
    MadeHand52 bestHand = MadeHand52(cards.begin(), cards.begin() + 5);
    for_each_combination(cards.begin(), cards.begin()+5, cards.end(),
    [&bestHand](const auto& first, const auto& last){
        MadeHand52 hand(first, last);
        if(bestHand < hand)
            bestHand = hand;
        return false;
    });
    return bestHand;
}

int compareHands(Hand left, Hand right)
{
    MadeHand52 bestLeft = getBestHand(left);
    MadeHand52 bestRight = getBestHand(right);
    if(bestLeft < bestRight) return -1;
    if(bestRight < bestLeft) return 1;
    return 0;
}

DrawingHand52::DrawingHand52(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end )
    : cards_(begin, end)
    , handRank_(DrawingHand52::None)
{
    if(cards_.size() != 4)
        throw std::runtime_error("drawing hand != size 4");
    for(auto& card : cards_)
        rankOccurences_[card.rank()]++;
    if(rankOccurences_.size() == 4) // HighCard, Flush, Straight or StraightFlush
    {
        // check for flush
        std::map<Suit, int> suit_occurences;
        for(const auto& card : cards_)
            suit_occurences[card.suit()]++;
        bool isFlush = suit_occurences.size() == 1;

        // check for straight except by wheel (A,2,3,4,5)
        auto compare = [](const auto& l, const auto& r){ return l.rank() < r.rank();};
        auto minRank = *std::min_element(cards_.begin(), cards_.end(), compare);
        auto maxRank = *std::max_element(cards_.begin(), cards_.end(), compare);
        bool isOpenend = (maxRank.rank() - minRank.rank()) == 3 and maxRank.rank() <= King;
        bool isGutshot = (maxRank.rank() - minRank.rank()) == 4;

        // check for wheel (A,2,3,4,5)
        auto compareWheel = [](const auto& l, const auto& r){ 
            int leftRank = l.rank();
            if(leftRank == Ace)
                leftRank -= 13;
            int rightRank = r.rank();
            if(rightRank == Ace)
                rightRank -= 13; 
            return leftRank < rightRank;
        };
        minRank = *std::min_element(cards_.begin(), cards_.end(), compareWheel);
        maxRank = *std::max_element(cards_.begin(), cards_.end(), compareWheel);
        bool isWheelGutshot = minRank.rank() == Ace and maxRank.rank() <= Five;


        // set the handrank
        if(not isWheelGutshot and not isGutshot and not isOpenend and not isFlush)
            handRank_ = None;
        if(isWheelGutshot and not isFlush)
            handRank_ = WheelGutshot;
        if(isGutshot and not isFlush)
            handRank_ = Gutshot;
        if(isOpenend and not isFlush)
            handRank_ = Openend;
        if(isWheelGutshot and isFlush)
            handRank_ = FlushWheelGutshot;
        if(isGutshot and isFlush)
            handRank_ = FlushGutshot;
        if(isOpenend and isFlush)
            handRank_ = FlushOpenend;
    }
}

std::vector<Rank52> DrawingHand52::getHighCards() const
{
    return cards_ | ranges::view::transform([](const auto& card){ return card.rank(); })
                  | ranges::to<std::vector>;
}

bool operator<(DrawingHand52 const& l, DrawingHand52 const& r)
{
    if(l.handRank_ != r.handRank_)
        return l.handRank_ < r.handRank_;
    return compareHighCards(l.getHighCards(), r.getHighCards());
}


bool operator<=(DrawingHand52 const& l, DrawingHand52 const& r)
{
    return not(l>r);
}

bool operator>(DrawingHand52 const& l, DrawingHand52 const& r)
{
    return r < l;
}

bool operator>=(DrawingHand52 const& l, DrawingHand52 const& r)
{
    return not(l<r);
}

bool operator==(DrawingHand52 const& l, DrawingHand52 const& r)
{
    return not (l<r) and not (r<l);
}

bool operator!=(DrawingHand52 const& l, DrawingHand52 const& r)
{
    return not (l == r);
}

}