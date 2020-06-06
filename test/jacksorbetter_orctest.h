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

#ifndef JACKSORBETTER_ORCTEST_H
#define JACKSORBETTER_ORCTEST_H

#include <QObject>
#include <QtTest/QtTest>

/**
 * @brief JacksOrBetter_OrcTest tests the orchestration of the Jacks-or-Better game with an account and card decks
 *        hands, holding, etc.
 */
class JacksOrBetter_OrcTest : public QObject
{
    Q_OBJECT
private slots:
    void testSingleHandGameNoHoldCards();
    void testSingleHandGame1HoldCard();
    void testSingleHandGameHoldAllCards();
    void testSingleHandMultipleGames();
    void testSingleHandInsufficientFunds();
    void testBetCycling();
    void testBetMaximum();
    void testBetPlayLose();
    void testBetPlayWinTwoPair();
};

#endif // JACKSORBETTER_ORCTEST_H
