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

#ifndef COMMONHANDANALYSIS_H
#define COMMONHANDANALYSIS_H

#include "hand.h"

/**
 * @file    This file contains common functions for poker hand analysis
 */
namespace HandAnalysis {

/**
 * @brief      This is a helper function. It won't validate a hand, but it is useful in the validation of hands.
 *             Sorts the hand's cards from lowest to highest value as it makes performing hand analysis easier
 *
 * @param[i|o] hand      Single poker game hand (will be sorted in place)
 */
void sortHandVector(QVector<PlayingCard> &hand);

/**
 * @brief      Helper function to determine if a hand has "N" of a kind (where N could be 2, 3, 4)
 *
 * @param[in]  nValue    The value of "N" in "N"-of-a-kind
 * @param[in]  hand      A single hand of a poker game
 * @param[out] matches   Vector of card values that were matched nValue times
 *
 * @return     number of times "N" of a kind was found in the hand
 *                 EXAMPLES: To match "Two Pair":         NOfAKind(2, hand) ==> 2
 *                           To match "Three of a Kind":  NOfAKind(3, hand) ==> 1
 */
quint8 NOfAKind(quint8 nValue, const QVector<PlayingCard> &hand, QVector<PlayingCard::CardValue> &matches);

/**
 * @brief      Determines if a hand meets Jacks-or-Better criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains a Jacks-or-Better
 */
bool JacksOrBetter(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets Two-Pair criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains 2 pairs
 */
bool TwoPair(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets Three-of-a-Kind criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains 3 of a kind
 */
bool ThreeOfAKind(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets a Straight criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains a straight
 */
bool Straight(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets a Flush criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand is a flush
 */
bool Flush(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets Full House criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains a full house
 */
bool FullHouse(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets Four-of-a-Kind criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains 4 of a kind
 */
bool FourOfAKind(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand meets a Straight Flush criteria
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand contains a straight flush
 */
bool StraightFlush(const QVector<PlayingCard> &hand);

/**
 * @brief      Determines if a hand is a Royal Flush
 *
 * @param[in]  hand      A single hand of a poker game
 *
 * @return     true if the hand is a royal flush
 */
bool RoyalFlush(const QVector<PlayingCard> &hand);

}  // namespace HandAnalysis

#endif // COMMONHANDANALYSIS_H
