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

#ifndef HAND_H
#define HAND_H

#include "playingcard.h"

#include <QVector>

/**
 * @brief A Hand is a set of 5 cards making up a poker hand. Cards may be inserted into a hand, held, and redrawn
 */
class Hand
{
public:
    /// Number of cards comprising a complete hand
    const static quint8 kCardsPerHand;

    /**
     * @brief      For some reason, old version of Qt containers do not like the explicit constructor and need to have
     *             a default one defined. You should NOT instantiate Deck items with this constructor! :)
     *
     * @warning    DO NOT USE THIS DIRECTLY, only included for Qt container compatibility
     */
    explicit Hand();

    /**
     * @brief      Creates a hand with cards all at once (pretty much only useful for unit testing as the actual game
     *             should add cards one at a time from a deck)
     *
     * @param[in]  card1      Hand playing card #1
     * @param[in]  card2      Hand playing card #2
     * @param[in]  card3      Hand playing card #3
     * @param[in]  card4      Hand playing card #4
     * @param[in]  card5      Hand playing card #5
     */
    explicit Hand(PlayingCard card1, PlayingCard card2, PlayingCard card3, PlayingCard card4, PlayingCard card5);

    /**
     * @brief      Adds a single card to the hand
     *
     * @param[in]  card       The single card to add to the hand
     *
     * @throws     runtime_error if too many cards are added ("Adding card will exceed hand limit")
     */
    void addCard(PlayingCard card);

    /**
     * @brief      Allows a client to copy the card at a specified hand position for comparisons
     *
     * @param[in]  cardIdx    Position from the hand to peek at the card
     *
     * @return     a const copy of the playing card
     */
    const PlayingCard cardAt(quint8 cardIdx) const;

    /**
     * @brief      Sets the hold status of a card in the hand
     *
     * @param[in]  cardNum    Index of the card to hold
     * @param[in]  hold       Set to true if the card should be held, false if it can be replaced with a draw
     *
     * @throws     runtime_error if the hand does not have all 5 cards ("Hand is not fully filled")
     */
    void holdCard(quint8 cardNum, bool hold);

    /**
     * @brief      Determines if a card at cardNum is held and should not be replaced with a draw card
     *
     * @param[in]  cardNum    Index of the card to query
     *
     * @return     true if card should not be replaced, false if it can be replaced with a draw card
     *
     * @throws     runtime_error if requested to hold a card not in the hand ("Request to hold card not in hand")
     */
    bool cardHeld(quint8 cardNum) const;

    /**
     * @brief      Replaces a card in the hand at cardNum with the new card
     *
     * @param[in]  cardNum    Index of the card in the hand to replace with 'card'
     * @param[in]  card       Card to replace the existing card at cardIdx
     *
     * @throws     runtime_error if requested cardNum is not in the hand ("Attempt to replace a card not in the hand")
     * @note       The card's replacement eligibility should be checked with cardHeld before using this function.
     */
    void replaceCard(quint8 cardNum, PlayingCard card);

    /**
     * @brief      Empties the hand so it can be repopulated
     */
    void reset();

    /**
     * @brief      Converts the set of cards in the hand into a QVector (so it can be optimized for hand analysis)
     *
     * @return     A QVector holding a copy of all cards in the hand
     */
    QVector<PlayingCard> handToVector() const;

private:
    // Each card in the hand.
    QVector<QPair<PlayingCard, bool>> _handCardStatus;
};

#endif // HAND_H
