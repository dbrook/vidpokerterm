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

Deck::Deck(DeckType typeOfDeck)
{
    /*
     * Populate the deck based on the type requested
     */
    if (typeOfDeck == FULL_FRENCH) {
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

    /*
     * Initialize a random number generator (RNG), provided by the Qt Framework. The first initialization should be
     * cryptographically secure (the global() guarantees this), but each card will actually be picked after with a
     * pseudo-random generator. True video poker machines have far more secure and robust RNGs, but this little program
     * is really just for fun.
     */
    _rand = QRandomGenerator(QRandomGenerator::global()->generate());
}

void Deck::shuffle()
{
    // TODO: is a shuffle really even required if we're using a stateful RNG + deck positions?
    //       see comment in Deck::drawCard. It might be worth changing the implementation of the deck...
}

PlayingCard Deck::drawCard()
{
    // TODO: This logic is messy. It could be better to properly shuffle the deck and then treat the deck as a stack>

    // There needs to actually be cards left (this condition should never happen)
    if (_drawnCardPos.size() == _cardDeck.size()) {
        throw std::runtime_error("No available cards in deck");
    }

    // Select a card at random from the vector (0 <= index < _cardDeck's size)
    // The QRandomNumber::bounded function guarantees uniform distribution
    quint32 deckPosition;
    do {
        deckPosition = _rand.bounded(_cardDeck.size());
    } while (_drawnCardPos.contains(deckPosition));

    // Blacklist the drawn card position (so it won't be picked again)
    _drawnCardPos.insert(deckPosition);

    // Give it back
    return _cardDeck[deckPosition];
}

void Deck::reset()
{
    _drawnCardPos.clear();
}
