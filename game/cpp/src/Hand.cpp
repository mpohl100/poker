#include "Hand.h"
#include "HoleCards.h"
#include "Board.h"

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

class ClassifiedHand{
public:
    enum HandRank52{
        HighCard,
        Pair,
        TwoPair,
        Trips,
        Straight,
        Flush,
        FullHouse,
        Quads,
        StraightFlush,
    };

    ClassifiedHand(std::vector<Card52> cards)
        : cards_(cards) 
    {
        if(cards.size() != 5)
            throw std::runtime_error("classified hand != size 5");
        for(auto& card : cards)
            rankOccurences_[card.rank()]++;
        if(rankOccurences_.size() == 5) // HighCard, Flush, Straight or StraightFlush
        {
            // check for flush
            std::map<Suit, int> suit_occurences;
            for(const auto& card : cards)
                suit_occurences[card.suit()]++;
            bool isFlush = suit_occurences.size() == 1;

            // check for straight except by wheel (A,2,3,4,5)
            auto compare = [](const auto& l, const auto& r){ return l.rank() < r.rank();};
            auto minRank = *std::min_element(cards.begin(), cards.end(), compare);
            auto maxRank = *std::max_element(cards.begin(), cards.end(), compare);
            bool isStraight = (maxRank.rank() - minRank.rank()) == 5;

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
            minRank = *std::min_element(cards.begin(), cards.end(), compareWheel);
            maxRank = *std::max_element(cards.begin(), cards.end(), compareWheel);
            isStraight = minRank.rank() == Ace and maxRank.rank() == Five;

            // set the handrank
            if(not isStraight and not isFlush)
                handRank_ = HighCard;
            if(isStraight)
                handRank_ = Straight;
            if(isFlush)
                handRank_ = Flush;
            if(isStraight and isFlush)
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

    std::array<Rank52,2> findOccurences(int nb) const
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

    int sum() const
    {
        return std::accumulate(cards_.begin(), cards_.end(), 0, [](int l, const Card52& r){return l + r.rank();});
    }

    std::vector<Card52> cards_;
    HandRank52 handRank_;
    std::map<Rank52, int> rankOccurences_;
};

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



int compareHands([[maybe_unused]] Hand left, [[maybe_unused]] Hand right)
{
    return 0;
}

}