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

#include "bonuspoker.h"

BonusPoker::BonusPoker() : PokerGame("Bonus Poker")
{
    _handPayouts = {
        {"Royal Flush",           {800, 1600, 2400, 3200, 4000}},   //  0
        {"Straight Flush",        { 50,  100,  150,  200,  250}},   //  1
        {"4 of a Kind (Aces)",    { 80,  160,  240,  320,  400}},   //  2
        {"4 of a Kind (2, 3, 4)", { 40,   80,  120,  160,  200}},   //  3
        {"4 of a Kind (5 to K)",  { 25,   50,   75,  100,  125}},   //  4
        {"Full House",            {  6,   12,   18,   24,   30}},   //  5
        {"Flush",                 {  5,   10,   15,   20,   25}},   //  6
        {"Straight",              {  4,    8,   12,   16,   20}},   //  7
        {"3 of a Kind",           {  3,    6,    9,   12,   15}},   //  8
        {"2 Pair",                {  2,    4,    6,    8,   10}},   //  9
        {"Jacks or Better",       {  1,    2,    3,    4,    5}},   // 10
        {"",                      {  0,    0,    0,    0,    0}}    // 11
    };
}

void BonusPoker::determineHandAndWin(const Hand &gameHand,
                                     quint32     nbCreditsBet,
                                     QString    &winningHand,
                                     quint32    &creditsWon)
{
    winningHand = "";
    creditsWon  = 0;
}
