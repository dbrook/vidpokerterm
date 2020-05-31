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

quint8 NOfAKind(quint8 nValue, const QVector<PlayingCard> &hand, QVector<PlayingCard::CardValue> &matches)
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
            matches.push_back(cardVal);
        }
    }
    return nbNValuesFound;
}


/*
 * Actual Hand Functions
 */
bool JacksOrBetter(const QVector<PlayingCard> &hand)
{
    QVector<PlayingCard::CardValue> matches;
    NOfAKind(2, hand, matches);

    // See if the matched pair was J+J, Q+Q, K+K, A+A
    for (const PlayingCard::CardValue &value : matches) {
        if (value == PlayingCard::ACE   || value == PlayingCard::KING ||
            value == PlayingCard::QUEEN || value == PlayingCard::JACK)
        {
            return true;
        }
    }
    return false;
}

bool TwoPair(const QVector<PlayingCard> &hand)
{
    QVector<PlayingCard::CardValue> matches;       // Unused for Three of a Kind
    return (NOfAKind(2, hand, matches) == 2);
}

bool ThreeOfAKind(const QVector<PlayingCard> &hand)
{
    QVector<PlayingCard::CardValue> matches;       // Unused for Three of a Kind
    return (NOfAKind(3, hand, matches) == 1);
}

bool Straight(const QVector<PlayingCard> &hand)
{
    PlayingCard::CardValue lowestCard = hand[0].value();

    for (qint32 handIdx = 1; handIdx < hand.size(); ++handIdx) {
        if (hand[handIdx].value() == lowestCard + 1) {
            lowestCard = hand[handIdx].value();
        } else {
            return false;
        }
    }

    // If control got out of the loop, then each card must have been higher than the last
    return true;
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
    QVector<PlayingCard::CardValue> matches;    // Unused for Full House
    return (NOfAKind(3, hand, matches) == 1 && NOfAKind(2, hand, matches) == 1);
}

bool FourOfAKind(const QVector<PlayingCard> &hand)
{
    QVector<PlayingCard::CardValue> matches;    // Unused for Full House
    return (NOfAKind(4, hand, matches) == 1);
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
