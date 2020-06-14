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

#include "pokerhand_test.h"

// Jacks or Better Payout and Hand Name tests
#include "jacksorbetter.h"

namespace {
// Test orchestrator function to hopefully make for less repetetive code
void validateResult(PokerGame &myGame, const Hand &myHand, quint8 credits, quint32 expWinnings, const QString &expHand)
{
    QString retHand;
    quint32 retWinnings;
    myGame.determineHandAndWin(myHand, credits, retHand, retWinnings);
    QCOMPARE(retWinnings, expWinnings);
    QCOMPARE(retHand, expHand);
}
}

void TestHands::testJOB_RoyalFlush() {
    JacksOrBetter JOB;

    /* Different bet amounts to test paytable validity, rearranging cards to test that the sorter algorithm works */
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::CLUB, PlayingCard::ACE  ),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::KING ),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::TEN  )),
                   1,                // Bet 1 credit
                   250,              // Expect 250 credits won
                   "Royal Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::SPADE, PlayingCard::ACE  ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::TEN  ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::KING )),
                   2,
                   500,
                   "Royal Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::HEART, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::KING ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::HEART, PlayingCard::ACE  ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::TEN  )),
                   3,
                   750,
                   "Royal Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::TEN  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::KING ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::ACE  )),
                   4,
                   1000,
                   "Royal Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::TEN  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::KING ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::ACE  )),
                   5,
                   4000,
                   "Royal Flush");
}

void TestHands::testJOB_StraightFlush()
{
    JacksOrBetter JOB;
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::HEART, PlayingCard::TWO  ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::THREE),
                        PlayingCard(PlayingCard::HEART, PlayingCard::FOUR ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::FIVE ),
                        PlayingCard(PlayingCard::HEART, PlayingCard::SIX  )),
                   5,
                   250,
                   "Straight Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::CLUB, PlayingCard::NINE ),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::SEVEN),
                        PlayingCard(PlayingCard::CLUB, PlayingCard::FIVE )),
                   4,
                   200,
                   "Straight Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::SPADE, PlayingCard::KING ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::TEN  ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::JACK ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::NINE )),
                   3,
                   150,
                   "Straight Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::SPADE, PlayingCard::FOUR ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::FIVE ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::THREE),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::SEVEN)),
                   2,
                   100,
                   "Straight Flush");

    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::FIVE )),
                   1,
                   50,
                   "Straight Flush");
}

void TestHands::testJOB_4OfAKind()
{
    JacksOrBetter JOB;

    /* Different bet amounts to test paytable validity only */
    Hand fourNines(PlayingCard(PlayingCard::HEART,   PlayingCard::NINE),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::NINE),
                   PlayingCard(PlayingCard::SPADE,   PlayingCard::NINE),
                   PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::FOUR));
    validateResult(JOB, fourNines, 5, 125, "4 of a Kind");
    validateResult(JOB, fourNines, 4, 100, "4 of a Kind");
    validateResult(JOB, fourNines, 3,  75, "4 of a Kind");
    validateResult(JOB, fourNines, 2,  50, "4 of a Kind");
    validateResult(JOB, fourNines, 1,  25, "4 of a Kind");

    /* Hand permutations (just care about making cases that test the 4-of-a-kind logic */
    Hand fourAces(PlayingCard(PlayingCard::HEART,   PlayingCard::ACE),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::NINE),
                  PlayingCard(PlayingCard::SPADE,   PlayingCard::ACE),
                  PlayingCard(PlayingCard::DIAMOND, PlayingCard::ACE),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE));
    validateResult(JOB, fourAces, 1, 25, "4 of a Kind");

    Hand fourTwos(PlayingCard(PlayingCard::HEART,   PlayingCard::TWO),
                  PlayingCard(PlayingCard::SPADE,   PlayingCard::TWO),
                  PlayingCard(PlayingCard::DIAMOND, PlayingCard::TWO),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::TWO));
    validateResult(JOB, fourTwos, 1, 25, "4 of a Kind");
}

void TestHands::testJOB_FullHouse()
{
    JacksOrBetter JOB;

    /* Full House paytable validity */
    Hand fhTensFullOfFives(PlayingCard(PlayingCard::DIAMOND, PlayingCard::TEN ),
                           PlayingCard(PlayingCard::HEART,   PlayingCard::FIVE),
                           PlayingCard(PlayingCard::HEART,   PlayingCard::TEN ),
                           PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE),
                           PlayingCard(PlayingCard::SPADE,   PlayingCard::TEN ));
    validateResult(JOB, fhTensFullOfFives, 5, 45, "Full House");
    validateResult(JOB, fhTensFullOfFives, 4, 36, "Full House");
    validateResult(JOB, fhTensFullOfFives, 3, 27, "Full House");
    validateResult(JOB, fhTensFullOfFives, 2, 18, "Full House");
    validateResult(JOB, fhTensFullOfFives, 1,  9, "Full House");

    /* Random permutations of Full House for matching tests */
    Hand fhQueensThrees(PlayingCard(PlayingCard::CLUB,    PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::THREE),
                        PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
                        PlayingCard(PlayingCard::CLUB,    PlayingCard::THREE));
    validateResult(JOB, fhQueensThrees, 1, 9, "Full House");

    Hand fhFivesSevens(PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE ),
                       PlayingCard(PlayingCard::HEART,   PlayingCard::FIVE ),
                       PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
                       PlayingCard(PlayingCard::SPADE,   PlayingCard::FIVE ),
                       PlayingCard(PlayingCard::CLUB,    PlayingCard::SEVEN));
    validateResult(JOB, fhFivesSevens, 1, 9, "Full House");
}

void TestHands::testJOB_Flush()
{
    JacksOrBetter JOB;

    /* Flush paytable validation */
    Hand allClubs(PlayingCard(PlayingCard::CLUB, PlayingCard::TEN  ),
                  PlayingCard(PlayingCard::CLUB, PlayingCard::FIVE ),
                  PlayingCard(PlayingCard::CLUB, PlayingCard::SEVEN),
                  PlayingCard(PlayingCard::CLUB, PlayingCard::ACE  ),
                  PlayingCard(PlayingCard::CLUB, PlayingCard::TWO  ));
    validateResult(JOB, allClubs, 5, 30, "Flush");
    validateResult(JOB, allClubs, 4, 24, "Flush");
    validateResult(JOB, allClubs, 3, 18, "Flush");
    validateResult(JOB, allClubs, 2, 12, "Flush");
    validateResult(JOB, allClubs, 1,  6, "Flush");

    /* Other flushes (spades, hearts, diamonds) */
    Hand allSpades(PlayingCard(PlayingCard::SPADE, PlayingCard::FOUR ),
                   PlayingCard(PlayingCard::SPADE, PlayingCard::FIVE ),
                   PlayingCard(PlayingCard::SPADE, PlayingCard::SIX  ),
                   PlayingCard(PlayingCard::SPADE, PlayingCard::TEN  ),
                   PlayingCard(PlayingCard::SPADE, PlayingCard::THREE));
    validateResult(JOB, allSpades, 1,  6, "Flush");

    Hand allHearts(PlayingCard(PlayingCard::HEART, PlayingCard::ACE  ),
                   PlayingCard(PlayingCard::HEART, PlayingCard::JACK ),
                   PlayingCard(PlayingCard::HEART, PlayingCard::TWO  ),
                   PlayingCard(PlayingCard::HEART, PlayingCard::QUEEN),
                   PlayingCard(PlayingCard::HEART, PlayingCard::SEVEN));
    validateResult(JOB, allHearts, 1,  6, "Flush");

    Hand allDiamonds(PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE ),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX  ));
    validateResult(JOB, allDiamonds, 1,  6, "Flush");
}

void TestHands::testJOB_StraightAceHigh()
{
    JacksOrBetter JOB;

    /* Paytable tests */
    Hand straightOnly(PlayingCard(PlayingCard::SPADE,   PlayingCard::KING ),
                      PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
                      PlayingCard(PlayingCard::HEART,   PlayingCard::JACK ),
                      PlayingCard(PlayingCard::HEART,   PlayingCard::TEN  ),
                      PlayingCard(PlayingCard::CLUB,    PlayingCard::NINE ));
    validateResult(JOB, straightOnly, 5, 20, "Straight");
    validateResult(JOB, straightOnly, 4, 16, "Straight");
    validateResult(JOB, straightOnly, 3, 12, "Straight");
    validateResult(JOB, straightOnly, 2,  8, "Straight");
    validateResult(JOB, straightOnly, 1,  4, "Straight");

    /* Some Straight permutations */
    Hand stTwoSix(PlayingCard(PlayingCard::SPADE,   PlayingCard::TWO  ),
                  PlayingCard(PlayingCard::DIAMOND, PlayingCard::THREE),
                  PlayingCard(PlayingCard::HEART,   PlayingCard::FOUR ),
                  PlayingCard(PlayingCard::HEART,   PlayingCard::FIVE ),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::SIX  ));
    validateResult(JOB, stTwoSix, 1,  4, "Straight");

    Hand stFiveNine(PlayingCard(PlayingCard::SPADE,   PlayingCard::NINE ),
                    PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
                    PlayingCard(PlayingCard::HEART,   PlayingCard::EIGHT),
                    PlayingCard(PlayingCard::HEART,   PlayingCard::SIX  ),
                    PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE ));
    validateResult(JOB, stFiveNine, 1,  4, "Straight");

    Hand stEightQueen(PlayingCard(PlayingCard::SPADE,   PlayingCard::NINE ),
                      PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                      PlayingCard(PlayingCard::HEART,   PlayingCard::TEN  ),
                      PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
                      PlayingCard(PlayingCard::CLUB,    PlayingCard::EIGHT));
    validateResult(JOB, stEightQueen, 1,  4, "Straight");

    Hand stTenAce(PlayingCard(PlayingCard::SPADE,   PlayingCard::TEN  ),
                  PlayingCard(PlayingCard::DIAMOND, PlayingCard::ACE  ),
                  PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
                  PlayingCard(PlayingCard::HEART,   PlayingCard::JACK ),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::KING ));
    validateResult(JOB, stTenAce, 1,  4, "Straight");
}

void TestHands::testJOB_StraightAceLow()
{
    JacksOrBetter JOB;
    Hand stAceFive(PlayingCard(PlayingCard::SPADE,   PlayingCard::FIVE ),
                   PlayingCard(PlayingCard::DIAMOND, PlayingCard::THREE),
                   PlayingCard(PlayingCard::HEART,   PlayingCard::TWO  ),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::FOUR ),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE  ));
    validateResult(JOB, stAceFive, 1,  4, "Straight");
}

void TestHands::testJOB_3OfAKind()
{
    JacksOrBetter JOB;

    /* Paytable tests */
    Hand threeSevens(PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE  ),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
                     PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE ),
                     PlayingCard(PlayingCard::CLUB,    PlayingCard::SEVEN),
                     PlayingCard(PlayingCard::HEART,   PlayingCard::SEVEN));
    validateResult(JOB, threeSevens, 5, 15, "3 of a Kind");
    validateResult(JOB, threeSevens, 4, 12, "3 of a Kind");
    validateResult(JOB, threeSevens, 3,  9, "3 of a Kind");
    validateResult(JOB, threeSevens, 2,  6, "3 of a Kind");
    validateResult(JOB, threeSevens, 1,  3, "3 of a Kind");

    Hand threeQueens(PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE  ),
                     PlayingCard(PlayingCard::DIAMOND, PlayingCard::QUEEN),
                     PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE ),
                     PlayingCard(PlayingCard::CLUB,    PlayingCard::QUEEN),
                     PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN));
    validateResult(JOB, threeSevens, 1,  3, "3 of a Kind");
}

void TestHands::testJOB_2Pair()
{
    JacksOrBetter JOB;
    Hand pairTwosEights(PlayingCard(PlayingCard::CLUB,  PlayingCard::TWO  ),
                        PlayingCard(PlayingCard::CLUB,  PlayingCard::KING ),
                        PlayingCard(PlayingCard::CLUB,  PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::TWO  ),
                        PlayingCard(PlayingCard::SPADE, PlayingCard::EIGHT));
    validateResult(JOB, pairTwosEights, 5, 10, "2 Pair");
    validateResult(JOB, pairTwosEights, 4,  8, "2 Pair");
    validateResult(JOB, pairTwosEights, 3,  6, "2 Pair");
    validateResult(JOB, pairTwosEights, 2,  4, "2 Pair");
    validateResult(JOB, pairTwosEights, 1,  2, "2 Pair");
}

void TestHands::testJOB_JacksOrBetter()
{
    JacksOrBetter JOB;
    Hand pairJacks(PlayingCard(PlayingCard::CLUB,    PlayingCard::JACK ),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::KING ),
                   PlayingCard(PlayingCard::SPADE,   PlayingCard::QUEEN),
                   PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                   PlayingCard(PlayingCard::CLUB,    PlayingCard::TEN  ));
    validateResult(JOB, pairJacks, 5, 5, "Jacks or Better");
    validateResult(JOB, pairJacks, 4, 4, "Jacks or Better");
    validateResult(JOB, pairJacks, 3, 3, "Jacks or Better");
    validateResult(JOB, pairJacks, 2, 2, "Jacks or Better");
    validateResult(JOB, pairJacks, 1, 1, "Jacks or Better");

    Hand pairQueens(PlayingCard(PlayingCard::HEART,   PlayingCard::ACE  ),
                    PlayingCard(PlayingCard::CLUB,    PlayingCard::QUEEN),
                    PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
                    PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                    PlayingCard(PlayingCard::HEART,   PlayingCard::TEN  ));
    validateResult(JOB, pairQueens, 1, 1, "Jacks or Better");

    Hand pairKings(PlayingCard(PlayingCard::SPADE,   PlayingCard::FIVE ),
                   PlayingCard(PlayingCard::DIAMOND, PlayingCard::KING ),
                   PlayingCard(PlayingCard::HEART,   PlayingCard::QUEEN),
                   PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK ),
                   PlayingCard(PlayingCard::HEART,   PlayingCard::KING ));
    validateResult(JOB, pairKings, 1, 1, "Jacks or Better");

    Hand pairAces(PlayingCard(PlayingCard::SPADE,   PlayingCard::TWO ),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::KING),
                  PlayingCard(PlayingCard::SPADE,   PlayingCard::ACE ),
                  PlayingCard(PlayingCard::DIAMOND, PlayingCard::JACK),
                  PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE ));
    validateResult(JOB, pairAces, 1, 1, "Jacks or Better");
}

void TestHands::testJOB_NoWin()
{
    JacksOrBetter JOB;
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::CLUB,    PlayingCard::FIVE ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN ),
                        PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE  ),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::JACK ),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::FIVE )),
                   5, 0, "");
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SEVEN),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE ),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::ACE  )),
                   5, 0, "");

    // Almost a straight flush, but not quite!
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::FIVE )),
                   1, 0, "");

    // Almost a straight flush, but not quite!
    validateResult(JOB,
                   Hand(PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::EIGHT),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::SPADE,   PlayingCard::SIX  ),
                        PlayingCard(PlayingCard::DIAMOND, PlayingCard::FIVE )),
                   1, 0, "");
    // TODO: how many tests does it make sense to do for "no win" hands?
}
