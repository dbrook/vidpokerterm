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

#include "deck.h"

#include <exception>

Deck::Deck()
{
}

Deck::Deck(DeckType typeOfDeck) : _typeOfDeck(typeOfDeck)
{
    /*
     * Populate the deck based on the type requested
     */
    this->reset();

    /*
     * Initialize a random number generator (RNG), provided by the Qt Framework. The first initialization should be
     * cryptographically secure (the global() guarantees this), but any calls to shuffle will use a pseudo-random
     * number generator based on this seed. True video poker machines have far more secure and robust RNGs, but this
     * is really just for fun.
     */
    _rand = QRandomGenerator(QRandomGenerator::global()->generate());
}

void Deck::shuffle()
{
    // For each card in the deck, pick a random location to swap cards using a classic "swap" code
    for (qint32 cardPosition = 0; cardPosition < _cardDeck.size(); ++cardPosition) {
        qint32      cardToSwapPosition  = _rand.bounded(_cardDeck.size());
        PlayingCard cardFromDestination = _cardDeck[cardToSwapPosition];
        _cardDeck[cardToSwapPosition]   = _cardDeck[cardPosition];
        _cardDeck[cardPosition]         = cardFromDestination;
    }
}

PlayingCard Deck::drawCard()
{
    if (_cardDeck.empty()) {
        throw std::runtime_error("No available cards in deck");
    }

    // Take a card off the end of the deck
    PlayingCard cardToGive = _cardDeck.last();
    _cardDeck.pop_back();

    // Give it back
    return cardToGive;
}

void Deck::removeCard(const PlayingCard &cardToRemove)
{
    qint32 removeCardPosition = _cardDeck.indexOf(cardToRemove);
    if (removeCardPosition != -1) {
        _cardDeck.remove(removeCardPosition);
    }
}

void Deck::addCard(const PlayingCard cardToInsert)
{
    _cardDeck.push_back(cardToInsert);
}

void Deck::reset()
{
    if (_typeOfDeck == FULL_FRENCH) {
        // Add all 52 cards of the Full French deck
        _cardDeck = {
            PlayingCard(PlayingCard::CLUB,    PlayingCard::TWO),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::THREE),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::FOUR),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::SIX),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::SEVEN),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::EIGHT),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::NINE),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::TEN),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::JACK),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::QUEEN),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::KING),
            PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE),

            PlayingCard(PlayingCard::SPADE,   PlayingCard::TWO),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::THREE),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::FOUR),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::FIVE),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::SIX),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::SEVEN),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::EIGHT),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::NINE),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::TEN),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::JACK),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::QUEEN),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::KING),
            PlayingCard(PlayingCard::SPADE,   PlayingCard::ACE),

            PlayingCard(PlayingCard::HEART,   PlayingCard::TWO),
            PlayingCard(PlayingCard::HEART,   PlayingCard::THREE),
            PlayingCard(PlayingCard::HEART,   PlayingCard::FOUR),
            PlayingCard(PlayingCard::HEART,   PlayingCard::FIVE),
            PlayingCard(PlayingCard::HEART,   PlayingCard::SIX),
            PlayingCard(PlayingCard::HEART,   PlayingCard::SEVEN),
            PlayingCard(PlayingCard::HEART,   PlayingCard::EIGHT),
            PlayingCard(PlayingCard::HEART,   PlayingCard::NINE),
            PlayingCard(PlayingCard::HEART,   PlayingCard::TEN),
            PlayingCard(PlayingCard::HEART,   PlayingCard::JACK),
            PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
            PlayingCard(PlayingCard::HEART,   PlayingCard::KING),
            PlayingCard(PlayingCard::HEART,   PlayingCard::ACE),

            PlayingCard(PlayingCard::DIAMOND, PlayingCard::TWO),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::THREE),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::FOUR),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::FIVE),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::TEN),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::KING),
            PlayingCard(PlayingCard::DIAMOND, PlayingCard::ACE),
        };
    }
}
