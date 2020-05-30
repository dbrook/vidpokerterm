/*
 * This file is part of VidPokerTerm. Copyright (c) 2020 Daniel Brook
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "commonhandanalysis.h"

#include <QMap>
#include <algorithm>

namespace HandAnalysis {

/*
 * HELPER FUNCTIONS
 */
void sortHandVector(QVector<PlayingCard> &hand)
{
    // Use a lambda-function for sorting (from lowest card value to highest) as allowed by std::sort
    std::sort(hand.begin(), hand.end(),
              [] (const PlayingCard &cardA, const PlayingCard &cardB) {
        return cardA.value() < cardB.value();
    });
}

quint8 NOfAKind(quint8 nValue, const QVector<PlayingCard> &hand)
{
    // Map to associate count of all cards in a hand
    QMap<PlayingCard::CardValue, quint8> cardValueCounts;

    // Count the occurrence of each card in the hand
    for (const PlayingCard &singleHandCard : hand) {
        cardValueCounts[singleHandCard.value()]++;
    }

    // Count and return the number of times nValue cards appeared
    quint8 nbNValuesFound = 0;
    for (const PlayingCard::CardValue &cardVal : cardValueCounts.keys()) {
        if (cardValueCounts[cardVal] == nValue) {
            ++nbNValuesFound;
        }
    }
    return nbNValuesFound;
}


/*
 * Actual Hand Functions
 */
bool JacksOrBetter(const QVector<PlayingCard> &hand)
{
    bool JOB = false;

    // TODO: Insert Jacks-Or-Better code

    return JOB;
}

bool TwoPair(const QVector<PlayingCard> &hand)
{
    // No card value occurred 4 times in the hand
    return (NOfAKind(2, hand) == 2);
}

bool ThreeOfAKind(const QVector<PlayingCard> &hand)
{
    return (NOfAKind(3, hand) == 1);
}

bool Straight(const QVector<PlayingCard> &hand)
{
    bool STR = false;

    // TODO: Insert code to find a straight here

    return STR;
}

bool Flush(const QVector<PlayingCard> &hand)
{
    PlayingCard::CardSuit firstSuit = hand[0].suit();
    for (const PlayingCard &currentCard : hand) {
        if (currentCard.suit() != firstSuit) {
            return false;
        }
    }
    return true;
}

bool FullHouse(const QVector<PlayingCard> &hand)
{
    return (NOfAKind(3, hand) == 1 && NOfAKind(2, hand) == 1);
}

bool FourOfAKind(const QVector<PlayingCard> &hand)
{
    return (NOfAKind(4, hand) == 1);
}

bool StraightFlush(const QVector<PlayingCard> &hand)
{
    return (Straight(hand) && Flush(hand));
}

bool RoyalFlush(const QVector<PlayingCard> &hand)
{
    // A Royal Flush must first be a Flush
    if (!Flush(hand)) {
        return false;
    }

    // Then it must have all cards in sequence: 10, J, Q, K, A
    if ((hand[0].value() == PlayingCard::TEN)   &&
        (hand[1].value() == PlayingCard::JACK)  &&
        (hand[2].value() == PlayingCard::QUEEN) &&
        (hand[3].value() == PlayingCard::KING)  &&
        (hand[4].value() == PlayingCard::ACE)) {
        return true;
    }
    return false;
}

}  // namespace HandAnalysis
