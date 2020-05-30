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

#ifndef POKERHAND_TEST_H
#define POKERHAND_TEST_H

#include <QObject>
#include <QtTest/QtTest>

class TestHands : public QObject
{
    Q_OBJECT
private slots:
    void testJOB_RoyalFlush();
    void testJOB_StraightFlush();
    void testJOB_4OfAKind();
    void testJOB_FullHouse();
    void testJOB_Flush();
    void testJOB_Straight();
    void testJOB_3OfAKind();
    void testJOB_2Pair();
    void testJOB_JacksOrBetter();
    void testJOB_NoWin();
};

#endif // POKERHAND_TEST_H
