#include "RatedHand.h"
#include "Hand.h"
#include "HowardsCombinatorics.h"

#include <cmath>

namespace game52{

RatedHand::RatedHand(HoleCards const& holeCards, Board const& board)
    : holeCards_(holeCards)
    , board_(board)
{}


float RatedHand::getScore() const
{
    float madeHandScore = getMadeHandScore();
    float drawingHandScore = getDrawingHandScore();
    return madeHandScore + drawingHandScore;
}

std::vector<Board> RatedHand::getBoards() const
{
    if(board_.getCards().size() == 5 )
        return { board_ };
    std::vector<Board> ret;
    Deck52 deck;
    deck.moveToFront(holeCards_.getCards());
    deck.moveToFront(board_.getCards());
    size_t remainingCardsNum = 5 - board_.getCards().size();
    if( remainingCardsNum < 5){
        std::vector<Card52> remainingCards = deck.getRemainingCards();
        for_each_combination(remainingCards.begin(), remainingCards.begin()+remainingCardsNum, remainingCards.end(),
        [this, &ret](const auto& first, const auto& last){
            std::vector<Card52> cards{first, last};
            Board board = board_;
            if(board.getCards().size() == 4)
                board.dealRiver(cards[0]);
            else if(board.getCards().size() == 3)
            {
                board.dealTurn(cards[0]);
                board.dealRiver(cards[1]);
            }
            ret.push_back(board);
            return false;
        });
    }
    else{
        for(size_t i = 0; i < nbPreflopBoards_; ++i){
            Deck52 localDeck = deck;
            localDeck.shuffleRemainder();
            Board board;
            board.dealFlop(localDeck.getFlop());
            board.dealTurn(localDeck.getCard());
            board.dealRiver(localDeck.getCard());
            ret.push_back(board);
        }
    }
    return ret;
}

double RatedHand::getSimulatedScore() const
{
    auto boards = getBoards();
    double sum = 0;
    for(const auto& board : boards){
        MadeHand52 madeHand = getBestHand(Hand{holeCards_, board});
        auto handRank = madeHand.getRank();
        double score = 0;
        switch (handRank)
        {
        case MadeHand52::HandRank52::HighCard:
            score = 0; break;
        case MadeHand52::HandRank52::Pair:
            score = 1; break;
        case MadeHand52::HandRank52::TwoPair:
            score = 2; break;
        case MadeHand52::HandRank52::Trips:
            score = 3; break;
        case MadeHand52::HandRank52::Straight:
            score = 4; break;
        case MadeHand52::HandRank52::Flush:
            score = 5; break;
        case MadeHand52::HandRank52::FullHouse:
            score = 6; break;
        case MadeHand52::HandRank52::Quads:
            score = 7; break;
        case MadeHand52::HandRank52::StraightFlush:
            score = 8; break;
        default:
            break;
        }
        auto handCards = madeHand.getCards();
        auto holeCards = holeCards_.getCards();
        std::vector<Card52> intersection;
        std::set_intersection(handCards.begin(), handCards.end(), holeCards.begin(), holeCards.end(), intersection.begin());
        if(intersection.size() == 1){
            Rank52 rank = intersection.front().rank();
            score += double(int(rank)) / 13 / 13;
        }
        else if(intersection.size() == 2){
            std::vector<int> ranks;
            for(const auto& card : intersection)
                ranks.push_back(int(card.rank()));
            std::sort(ranks.begin(), ranks.end(), std::greater{});
            score += double(ranks[0]) / 13 + double(ranks[1]) / 13 / 13;
        }
        sum += score;
    }
    return sum / double(boards.size());
}

float RatedHand::getMadeHandScore() const
{
    MadeHand52 madeHand = getBestHand(Hand(holeCards_, board_));
    Rank52 relevantCard = madeHand.getRelevantCard();
    Rank52 kicker = madeHand.getKicker();
    float baseScore = 13*13*int(madeHand.handRank_);
    return baseScore + 13*int(relevantCard) + int(kicker);
}

float RatedHand::getDrawingHandScore() const
{
    AllDraws allDraws(Hand(holeCards_, board_));
    float topPair = int(Ten)*13 + int(Jack) + 13*13*int(MadeHand52::Pair);
    int outs = allDraws.getOuts();
    topPair *= outs / 9.0;
    return int(topPair);
}

}