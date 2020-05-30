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

#include "pokergame.h"

#include "commonhandanalysis.h"

#include <exception>

PokerGame::PokerGame()
{
    setCreditsPerBet(1);
    reset();
}

PokerGame::~PokerGame() {}

void PokerGame::setCreditsPerBet(quint8 credits)
{
    if (credits == 0 || credits > 5) {
        throw std::runtime_error("Invalid number of credits for game");
    }

    _credPerBet = credits;
}

quint8 PokerGame::getCreditsPerBet() const          {return _credPerBet;}

void PokerGame::setHandResult(QString handResult)   {_handResult = handResult;}

QString PokerGame::handResult() const               {return _handResult;}

void PokerGame::setWinnings(quint32 winningCredits) {_winnings = winningCredits;}

quint32 PokerGame::getWinnings() const              {return _winnings;}

void PokerGame::reset()
{
    _handResult = "";
    _winnings   = 0;
    setCreditsPerBet(1);
}
