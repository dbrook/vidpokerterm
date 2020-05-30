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

#ifndef DECK_H
#define DECK_H

#include "playingcard.h"

#include <QRandomGenerator>
#include <QSet>
#include <QVector>

/**
 * @brief A Deck is a collection of playing cards from which cards may be dealt one-by-one and shuffled with a RNG.
 *        Cards drawn from the deck are not actually removed from the vector, but instead their indexes in the vector
 *        are added to a black-list (so you cannot draw the same card multiple times)
 *
 * @note  The RNG used will be initialized in a cryptographically-secure way, but subsequent calls use the pseudo
 *        RNG after this first initialization. Real hardware-based RNGs are used in actual video poker terminals, but
 *        this is basically just for fun :)
 */
class Deck
{
public:
    /**
     * @brief DeckType represents the type of a deck to create
     */
    enum DeckType {
        FULL_FRENCH     // 52-card full French style playing card deck, no jokers
                        // TODO: maybe other decks could be supported (i.e. with jokers for some games)
    };

    /**
     * @brief      Construct a deck of a given type
     *
     * @param[in]  typeOfDeck
     */
    explicit Deck(DeckType typeOfDeck);

    /**
     * @brief      Shuffles the cards using a cryptographically-secure (per Qt) random number generator
     */
    void shuffle();

    /**
     * @brief      DrawCard finds a random card from the deck
     *
     * @throws     runtime_error if there are no cards available ("No available cards in deck")
     * @return     A single PlayingCard from the deck
     */
    PlayingCard drawCard();

    /**
     * @brief      Blacklists a card from being drawable when using drawCard. This can be handy in multi-hand games
     *             where a player would select hold cards from one hand and the hold card(s) must be propagated to all
     *             other hands in the game
     *
     * @param[in]  cardToRemove    Card to mark as being unable to get drawn by drawCard
     */
    void removeCard(const PlayingCard &cardToRemove);

    /**
     * @brief      Resets the internal black list, restoring the deck back to full eligibility
     */
    void reset();

private:
    /* Data members */
    QVector<PlayingCard>          _cardDeck;      // Vector of all cards in the deck
    QSet<quint32>                 _drawnCardPos;  // Set of indexes that were drawn
    QRandomGenerator              _rand;          // Random number generator for shuffling and drawing cards
};

#endif // DECK_H
