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

#include "hand.h"

#include <exception>

// All poker hands (at least so far...) are presumed to have 5 cards.
// This parameter is to avoid using "magic numbers" in the code (instead referring to kCardsPerHand)
const quint8 Hand::kCardsPerHand = 5;

Hand::Hand()
{
    // Claim 5 spaces for cards
    _handCardStatus.reserve(kCardsPerHand);
}

Hand::Hand(PlayingCard card1, PlayingCard card2, PlayingCard card3, PlayingCard card4, PlayingCard card5)
{
    addCard(card1);
    addCard(card2);
    addCard(card3);
    addCard(card4);
    addCard(card5);
}

void Hand::addCard(PlayingCard card)
{
    if (_handCardStatus.size() == kCardsPerHand) {
        throw std::runtime_error("Adding card will exceed hand limit");
    }
    _handCardStatus.push_back({card, false});
}

void Hand::holdCard(quint8 cardNum, bool hold)
{
    if (_handCardStatus.size() != kCardsPerHand) {
        throw std::runtime_error("Hand is not fully filled");
    }
    _handCardStatus[cardNum].second = hold;
}

bool Hand::cardHeld(quint8 cardNum) const
{
    if (cardNum >= _handCardStatus.size()) {
        throw std::runtime_error("Request to hold card not in hand");
    }
    return _handCardStatus[cardNum].second;
}

void Hand::replaceCard(quint8 cardNum, PlayingCard card)
{
    if (cardNum >= _handCardStatus.size()) {
        throw std::runtime_error("Attempt to replace a card not in the hand");
    }
    _handCardStatus[cardNum].first = card;
}

QVector<PlayingCard> Hand::handToVector() const
{
    QVector<PlayingCard> cards;
    for (const QPair<PlayingCard, bool> &singleCard : _handCardStatus) {
        cards.push_back(singleCard.first);
    }
    return cards;
}
