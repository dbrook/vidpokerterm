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

#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

/**
 * @brief A PlayingCard is a single card with a suit and value (rank)
 */
class PlayingCard
{
public:
    enum CardSuit {
        DIAMOND,
        HEART,
        SPADE,
        CLUB
    };

    enum CardValue {
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE
    };

    PlayingCard();

    /**
     * @brief      A single card consisting of a suit and value
     *
     * @param[in]  cardSuit
     * @param[in]  cardValue
     */
    explicit PlayingCard(CardSuit cardSuit, CardValue cardValue);

    /**
     * @brief operator == overloading for use by the Deck's removeCard() feature
     */
    bool operator==(const PlayingCard &card) const;

    /**
     * @brief      Getter for card's suit
     *
     * @return     Suit of the individual card
     */
    CardSuit suit() const;

    /**
     * @brief      Getter for card's value
     *
     * @return     Value of the individual card
     */
    CardValue value() const;

private:
    CardSuit  _suit;
    CardValue _value;
};

#endif // PLAYINGCARD_H
