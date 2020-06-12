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

PokerGame::PokerGame(const QString &gameName) : _gameName(gameName) {}

PokerGame::~PokerGame() {}

const QString PokerGame::gameName() const {return _gameName;}

bool PokerGame::creditBetValid(quint32 nbCredPerBet) const
{
    if (nbCredPerBet == 0 || nbCredPerBet > 5) {
        return false;
    }
    return true;
}

void PokerGame::currentPayTable(quint32 nbCreditsPerBet, QVector<QPair<const QString, int>> &payoutForBet) const
{
    if (!creditBetValid(nbCreditsPerBet)) {
        throw std::runtime_error("Invalid number of credits for game");
    }

    for (const Parameters &singleHand : _handPayouts) {
        QPair<QString, int> handPayout(singleHand.handString, singleHand.payoutCredits[nbCreditsPerBet - 1]);
        payoutForBet.push_back(handPayout);
    }
}
