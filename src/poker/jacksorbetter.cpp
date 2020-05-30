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

JacksOrBetter::JacksOrBetter() : PokerGame()
{
    _jacksOrBetter = {
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

void JacksOrBetter::analyzeHand(const Hand &gameHand)
{
    // Convert the hand to a vector so that algorithms can re-sort the cards as needed to efficiently determine wins
    QVector<PlayingCard> singleHand = gameHand.handToVector();

    // Sort the hand by order of highest to lowest card rank, as required by some hand analysis algorithms
    HandAnalysis::sortHandVector(singleHand);

    // Find the credits actually bet so the correct payout column will be chosen
    quint8 payoutArrayIdx = getCreditsPerBet() - 1;

    // Go in order of best hand to nothing, so we ensure the best winnings possible are met
    if (HandAnalysis::RoyalFlush(singleHand)) {
        setHandResult(_jacksOrBetter[0].handString);
        setWinnings(_jacksOrBetter[0].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::StraightFlush(singleHand)) {
        setHandResult(_jacksOrBetter[1].handString);
        setWinnings(_jacksOrBetter[1].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::FourOfAKind(singleHand)) {
        setHandResult(_jacksOrBetter[2].handString);
        setWinnings(_jacksOrBetter[2].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::FullHouse(singleHand)) {
        setHandResult(_jacksOrBetter[3].handString);
        setWinnings(_jacksOrBetter[3].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::Flush(singleHand)) {
        setHandResult(_jacksOrBetter[4].handString);
        setWinnings(_jacksOrBetter[4].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::Straight(singleHand)) {
        setHandResult(_jacksOrBetter[5].handString);
        setWinnings(_jacksOrBetter[5].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::ThreeOfAKind(singleHand)) {
        setHandResult(_jacksOrBetter[6].handString);
        setWinnings(_jacksOrBetter[6].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::TwoPair(singleHand)) {
        setHandResult(_jacksOrBetter[7].handString);
        setWinnings(_jacksOrBetter[7].payoutCredits[payoutArrayIdx]);
    } else if (HandAnalysis::JacksOrBetter(singleHand)) {
        setHandResult(_jacksOrBetter[8].handString);
        setWinnings(_jacksOrBetter[8].payoutCredits[payoutArrayIdx]);
    } else {
        setHandResult(_jacksOrBetter[9].handString);
        setWinnings(_jacksOrBetter[9].payoutCredits[payoutArrayIdx]);
    }
}
