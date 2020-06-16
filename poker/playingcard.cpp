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

#include "playingcard.h"

PlayingCard::PlayingCard() : _null(true) {}

PlayingCard::PlayingCard(CardSuit cardSuit, CardValue cardValue) : _null(false), _suit(cardSuit), _value(cardValue) {}

bool PlayingCard::fakeCard() const {return _null;}

bool PlayingCard::operator==(const PlayingCard &card) const
{
    return ((this->fakeCard() && card.fakeCard()) ||
            (!this->fakeCard() &&
             !card.fakeCard()  &&
             (this->suit() == card.suit()) &&
             (this->value() == card.value())));
}

PlayingCard::CardSuit PlayingCard::suit() const   {return _suit;}

PlayingCard::CardValue PlayingCard::value() const {return _value;}
