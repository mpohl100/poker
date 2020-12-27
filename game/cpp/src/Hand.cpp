#include "Hand.h"
#include "HoleCards.h"
#include "Board.h"
#include "HowardsCombinatorics.h"


#include <ranges>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <numeric>
#include <array>

namespace game52 {

Hand::Hand(Player* player, HoleCards const& holeCards, Board const& board)
    : player_(player)
{
    cards_ = holeCards.getCards();
    const auto& boardCards = board.getCards();
    cards_.insert(cards_.end(), boardCards.begin(), boardCards.end());
}

std::vector<Card52> Hand::getCards() const
{
    return cards_;
}

ClassifiedHand::ClassifiedHand(std::vector<Card52>::iterator begin, std::vector<Card52>::iterator end )
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

std::array<Rank52,2> ClassifiedHand::findOccurences(int nb) const
{
    std::array<Rank52,2> ret;
    int index = 0;
    for(const auto& [rank, occ] : rankOccurences_)
        if(occ == nb)
            ret[index++] = rank;
    if(ret[0] < ret[1])
        std::swap(ret[0], ret[1]);
    return ret;
};

int ClassifiedHand::sum() const
{
    return std::accumulate(cards_.begin(), cards_.end(), 0, [](int l, const Card52& r){return l + r.rank();});
}

ClassifiedHand ClassifiedHand::fromString(std::string const& str)
{
    auto cards =  str | std::ranges::views::split(' ')
                      | std::ranges::views::transform([](auto &&rng) {
        return std::string(&*rng.begin(), std::ranges::distance(rng));
    });
    std::vector<Card52> hand;
    for(auto card : cards)
        hand.push_back(Card52(card));
    return ClassifiedHand(hand.begin(), hand.end());
}


bool operator<(ClassifiedHand const& l, ClassifiedHand const& r)
{
    if(l.handRank_ != r.handRank_)
        return l.handRank_ < r.handRank_;
    // detailed looking into the higher hand

    // high card
    if(l.handRank_ == ClassifiedHand::HighCard)
    {
        return l.sum() < r.sum();
    }
    if(l.handRank_ == ClassifiedHand::Pair)
    {
        auto lpairRank = l.findOccurences(2);
        auto rPairRank = r.findOccurences(2);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return l.sum() < r.sum(); // if pairs are even, the higher card wins
    }
    if(l.handRank_ == ClassifiedHand::TwoPair)
    {
        auto lpairRank = l.findOccurences(2);
        auto rPairRank = r.findOccurences(2); 
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        if(lpairRank[1] != rPairRank[1])
            return lpairRank[1] < rPairRank[1];
        return l.sum() < r.sum(); // if pairs are even, the higher card wins
    }
    if(l.handRank_ == ClassifiedHand::Trips)
    {
        auto lpairRank = l.findOccurences(3);
        auto rPairRank = r.findOccurences(3);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return l.sum() < r.sum(); // if trips are even, the higher card wins
    }
    if(l.handRank_ == ClassifiedHand::Straight)
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
    if(l.handRank_ == ClassifiedHand::Flush)
    {
        return l.sum() < r.sum();
    }
    if(l.handRank_ == ClassifiedHand::FullHouse)
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
        return l.sum() < r.sum(); // if pairs are even, the higher card wins
    }
    if(l.handRank_ == ClassifiedHand::Quads)
    {
        auto lpairRank = l.findOccurences(4);
        auto rPairRank = r.findOccurences(4);
        if(lpairRank[0] != rPairRank[0])
            return lpairRank[0] < rPairRank[0];
        return l.sum() < r.sum(); // if pairs are even, the higher card wins
    }
    if( l.handRank_ == ClassifiedHand::StraightFlush)
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

bool operator<=(ClassifiedHand const& l, ClassifiedHand const& r)
{
    return not(l>r);
}

bool operator>(ClassifiedHand const& l, ClassifiedHand const& r)
{
    return r < l;
}

bool operator>=(ClassifiedHand const& l, ClassifiedHand const& r)
{
    return not(l<r);
}

bool operator==(ClassifiedHand const& l, ClassifiedHand const& r)
{
    return not (l<r) and not (r<l);
}


bool operator!=(ClassifiedHand const& l, ClassifiedHand const& r)
{
    return not (l == r);
}

ClassifiedHand getBestHand(Hand hand)
{ 
    auto cards = hand.getCards();
    ClassifiedHand bestHand = ClassifiedHand(cards.begin(), cards.begin() + 5);
    for_each_combination(cards.begin(), cards.begin()+5, cards.end(),
    [&bestHand](const auto& first, const auto& last){
        ClassifiedHand hand(first, last);
        if(bestHand < hand)
            bestHand = hand;
        return false;
    });
    return bestHand;
}

int compareHands([[maybe_unused]] Hand left, [[maybe_unused]] Hand right)
{
    ClassifiedHand bestLeft = getBestHand(left);
    ClassifiedHand bestRight = getBestHand(right);
    if(bestLeft < bestRight) return -1;
    if(bestRight < bestLeft) return 1;
    return 0;
}

}