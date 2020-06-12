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

#include "jacksorbetter.h"

#include "commonhandanalysis.h"

JacksOrBetter::JacksOrBetter() : PokerGame("Jacks or Better")
{
    _handPayouts = {
        {"Royal Flush",     {  250,  500,  750, 1000, 4000}},   // 0
        {"Straight Flush",  {   50,  100,  150,  200,  250}},   // 1
        {"4 of a Kind",     {   25,   50,   75,  100,  125}},   // 2
        {"Full House",      {    9,   18,   27,   36,   45}},   // 3
        {"Flush",           {    6,   12,   18,   24,   30}},   // 4
        {"Straight",        {    4,    8,   12,   16,   20}},   // 5
        {"3 of a Kind",     {    3,    6,    9,   12,   15}},   // 6
        {"2 Pair",          {    2,    4,    6,    8,   10}},   // 7
        {"Jacks or Better", {    1,    2,    3,    4,    5}},   // 8
        {"",                {    0,    0,    0,    0,    0}}    // 9
    };
}

void JacksOrBetter::determineHandAndWin(const Hand &gameHand,
                                        quint32     nbCreditsBet,
                                        QString    &winningHand,
                                        quint32    &creditsWon)
{
    // Convert the hand to a vector so that algorithms can re-sort the cards as needed to efficiently determine wins
    QVector<PlayingCard> singleHand = gameHand.handToVector();

    // Sort the hand by order of highest to lowest card rank, as required by some hand analysis algorithms
    HandAnalysis::sortHandVector(singleHand);

    // Go in order of best hand to nothing, so we ensure the best winnings possible are met
    if (HandAnalysis::RoyalFlush(singleHand)) {
        winningHand = _handPayouts[0].handString;
        creditsWon  = _handPayouts[0].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::StraightFlush(singleHand)) {
        winningHand = _handPayouts[1].handString;
        creditsWon  = _handPayouts[1].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::FourOfAKind(singleHand)) {
        winningHand = _handPayouts[2].handString;
        creditsWon  = _handPayouts[2].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::FullHouse(singleHand)) {
        winningHand = _handPayouts[3].handString;
        creditsWon  = _handPayouts[3].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::Flush(singleHand)) {
        winningHand = _handPayouts[4].handString;
        creditsWon  = _handPayouts[4].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::Straight(singleHand)) {
        winningHand = _handPayouts[5].handString;
        creditsWon  = _handPayouts[5].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::ThreeOfAKind(singleHand)) {
        winningHand = _handPayouts[6].handString;
        creditsWon  = _handPayouts[6].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::TwoPair(singleHand)) {
        winningHand = _handPayouts[7].handString;
        creditsWon  = _handPayouts[7].payoutCredits[nbCreditsBet - 1];
    } else if (HandAnalysis::JacksOrBetter(singleHand)) {
        winningHand = _handPayouts[8].handString;
        creditsWon  = _handPayouts[8].payoutCredits[nbCreditsBet - 1];
    } else {
        winningHand = _handPayouts[9].handString;
        creditsWon  = _handPayouts[9].payoutCredits[nbCreditsBet - 1];
    }
}
