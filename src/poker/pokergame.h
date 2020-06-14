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

#ifndef POKERGAME_H
#define POKERGAME_H

#include "hand.h"

#include <QVector>

/**
 * @brief This is a pure-virtual class representing the core functions of a game. This should be inherited from to
 *        create an actual game.
 */
class PokerGame
{
public:
    /// Hand Parameters (ID, string representaiton, payout level, credits)
    struct Parameters {
        QString          handString;
        QVector<quint32> payoutCredits;
    };

    /**
     * @brief Abstract game paytable and analysis class. See jacksorbetter.h/cpp files to see how to create a game
     */
    explicit PokerGame(const QString &gameName);

    /**
     * @brief Virtual destructor
     */
    virtual ~PokerGame();

    /**
     * @brief gameName returns the name of the game that was instantiated
     *
     * @return string representation of the actual poker game hand analysis used
     */
    const QString gameName() const;

    /**
     * @brief determineHandAndWin is where the actual hand evaluation logic and winnings shall be determined by classes
     *                            inheriting from this abstract base class
     *
     * @param[in]  gameHand       A set of PlayingCards comprising the player's hand construction
     * @param[in]  nbCreditsBet   Number of credits the player has staked for gameHand
     * @param[out] winningHand    QString buffer to write the winning hand (if any)
     * @param[out] creditsWon     Number of credits the player is entitled to win based on the gameHand and bet
     */
    virtual void determineHandAndWin(const Hand &gameHand,
                                     quint32     nbCreditsBet,
                                     QString    &winningHand,
                                     quint32    &creditsWon) = 0;

    /**
     * @brief currentPayTable extracts the relevant paytable based on how many credits would be bet
     *
     * @param[in]  nbCredPerBet   number of credits that would be bet
     * @param[out] payoutForBet   vector of strings (name of hand) and int (payout value for that hand)
     */
    void currentPayTable(quint32 nbCredPerBet, QVector<QPair<const QString, int> > &payoutForBet) const;

protected:
    /**
     * @brief creditBetValid determines if a valid number of credits was requested for betting
     *
     * @param[in]  nbCredPerBet   Number of credits the player would like to bet
     *
     * @return     true if the number of credits are supported, false otherwise
     */
    bool creditBetValid(quint32 nbCredPerBet) const;

    /**
     * @brief _handPayouts is where sub classes inheriting from a PokerGame should save their payout tables
     */
    QVector<Parameters> _handPayouts;

private:
    QString             _gameName;
};

#endif // POKERGAME_H
