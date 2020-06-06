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
     * @brief      ~
     */
    explicit PokerGame(const QString &gameName);

    /**
     * @brief      Virtual destructor
     */
    virtual ~PokerGame();

    /**
     * @brief gameName returns the name of the game that was instantiated
     *
     * @return string representation of the actual poker game hand analysis used
     */
    const QString gameName() const;

    /**
     * @brief      Choose the pay table
     *
     * @param[in]  credits     Credit / bet to spend towards each hand - must be between 1 and 5
     *
     * @throws     runtime_error if an invalid number of credits was proposed ("Invalid number of credits for game")
     */
    void setCreditsPerBet(quint8 credits);

    /**
     * @brief      Gets current pay table selection
     *
     * @return     number of credits currently bet
     */
    quint8 getCreditsPerBet() const;

    /**
     * @brief      Analyze a hand to see what (if any) winnings will apply
     *
     * @param[in]  gameHand    Reference to the hand that the game currently has
     */
    virtual void analyzeHand(const Hand &gameHand) = 0;

    /**
     * @brief      handResult
     *
     * @return     Hardcoded string representing the results of the hand analysis
     *
     * @note       analyzeHand must be run first
     */
    QString handResult() const;

    /**
     * @brief      Determines how many credits should be awarded to the player given the credits bet and hand result
     *
     * @return     Number of credits won per the final hand
     *
     * @note       analyzeHand must be run first
     */
    quint32 getWinnings() const;

    /**
     * @brief      Resets the hand processor
     */
    void reset();

    /**
     * @brief currentPayTable extracts the relevant paytable based on the current creditsPerBet value
     *
     * @param[out] payoutForBet   vector of strings (name of hand) and int (payout value for that hand)
     */
    void currentPayTable(QVector<QPair<const QString, int> > &payoutForBet) const;

protected:
    /**
     * @brief      handResult
     *
     * @param[in]  Hardcoded string representing the results of the hand analysis
     */
    void setHandResult(QString handResult);

    /**
     * @brief      Determines how many credits should be awarded to the player given the credits bet and hand result
     *
     * @return     Number of credits won per the final hand
     *
     * @note       analyzeHand must be run first
     */
    void setWinnings(quint32 winningCredits);

    /**
     * @brief _handPayouts is where sub classes inheriting from a PokerGame should save their payout tables
     */
    QVector<Parameters> _handPayouts;

private:
    QString             _gameName;
    quint8              _credPerBet;
    quint32             _winnings;
    QString             _handResult;
};

#endif // POKERGAME_H
