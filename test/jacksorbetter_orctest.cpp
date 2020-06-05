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

#include "jacksorbetter_orctest.h"

#include "account.h"
#include "gameorchestrator.h"
#include "jacksorbetter.h"

namespace {
/**
 * @brief initAndPlayOneRound initializes a game, then holds the cards requested and returns the first hand (before
 *        the holds were set) and the final hand (the result of replacing cards in the deck that were not held)
 *
 * @param[in]  cardsToHold   Vector of true/false for each position/card to hold or let get re-dealt
 * @param[out] firstHand     all the cards from the initial draw
 * @param[out] finalHand     all the cards in the final hand (after holding and drawing)
 */
void initAndPlayOneRound(const QVector<bool> &cardsToHold, Hand &firstHand, Hand &finalHand) {
    // Build a game with an account balance and inspect the individual steps of the game progress
    Account playerAcct;
    playerAcct.add(1000);

    JacksOrBetter    gameJOB;
    GameOrchestrator orcJOB(gameJOB, 1, playerAcct);

    // Deal the cards from the internal random deck
    orcJOB.dealDraw();

    // Inspect the cards (return them for analysis)
    firstHand = orcJOB.retrieveHand(0);

    // Hold any cards at the position requested
    for (qint32 cardIdx = 0; cardIdx < cardsToHold.size(); ++cardIdx) {
        if (cardsToHold[cardIdx] == true) {
            orcJOB.hold(cardIdx, true);
        }
    }

    // Draw cards that were not held
    orcJOB.dealDraw();

    // Return final hand for comparison
    finalHand = orcJOB.retrieveHand(0);
}

QVector<bool> determineSameCards(const Hand &firstHand, const Hand &finalHand) {
    QVector<bool> actualSameCards;

    QVector<PlayingCard> firstVec = firstHand.handToVector();
    QVector<PlayingCard> finalVec = finalHand.handToVector();

    for (qint32 cardIdx = 0; cardIdx < firstVec.size(); ++cardIdx) {
        if (firstVec[cardIdx] == finalVec[cardIdx]) {
            actualSameCards.push_back(true);
        } else {
            actualSameCards.push_back(false);
        }
    }

    return actualSameCards;
}
}

void JacksOrBetter_OrcTest::testSingleHandGameNoHoldCards()
{
    QVector<bool> expectedSameCards = {false, false, false, false, false};
    Hand          firstHand, finalHand;
    initAndPlayOneRound(expectedSameCards, firstHand, finalHand);
    QVector<bool> actualSameCards = determineSameCards(firstHand, finalHand);

    QCOMPARE(expectedSameCards, actualSameCards);
}

void JacksOrBetter_OrcTest::testSingleHandGame1HoldCard()
{
    QVector<bool> expectedSameCards = {true, false, false, false, false};
    Hand          firstHand, finalHand;
    initAndPlayOneRound(expectedSameCards, firstHand, finalHand);
    QVector<bool> actualSameCards = determineSameCards(firstHand, finalHand);

    QCOMPARE(expectedSameCards, actualSameCards);
}

void JacksOrBetter_OrcTest::testSingleHandGameHoldAllCards()
{
    QVector<bool> expectedSameCards = {true, true, true, true, true};
    Hand          firstHand, finalHand;
    initAndPlayOneRound(expectedSameCards, firstHand, finalHand);
    QVector<bool> actualSameCards = determineSameCards(firstHand, finalHand);

    QCOMPARE(expectedSameCards, actualSameCards);
}

void JacksOrBetter_OrcTest::testSingleHandMultipleGames()
{
    // Build a game with an account balance and inspect the individual steps of the game progress
    // Sorry this is repeated code, but it was important to test that a game can continue more than 1 deal+draw!
    Account playerAcct;
    playerAcct.add(1000);

    JacksOrBetter    gameJOB;
    GameOrchestrator orcJOB(gameJOB, 1, playerAcct);

    // Deal the cards from the internal random deck
    orcJOB.dealDraw();

    // Inspect the cards
    Hand firstHand = orcJOB.retrieveHand(0);

    // Hold 1 cards and redraw
    orcJOB.hold(0, true);
    orcJOB.hold(1, true);
    orcJOB.hold(3, true);
    orcJOB.hold(4, true);
    orcJOB.dealDraw();
    Hand finalHand = orcJOB.retrieveHand(0);

    // Inspect the new cards, they all should be different than what we had before
    QVector<bool>        expectedSameCards = {true, true, false, true, true};
    QVector<bool>        actualSameCards;
    QVector<PlayingCard> firstVec          = firstHand.handToVector();
    QVector<PlayingCard> finalVec          = finalHand.handToVector();

    for (qint32 cardIdx = 0; cardIdx < firstVec.size(); ++cardIdx) {
        if (firstVec[cardIdx] == finalVec[cardIdx]) {
            actualSameCards.push_back(true);
        } else {
            actualSameCards.push_back(false);
        }
    }

    QCOMPARE(expectedSameCards, actualSameCards);

    // Reset and deal again, this time only hold the other card to ensure the other holds are reset in new round
    orcJOB.dealDraw();
    firstHand = orcJOB.retrieveHand(0);
    orcJOB.hold(2, true);
    orcJOB.dealDraw();
    finalHand = orcJOB.retrieveHand(0);
    expectedSameCards = {false, false, true, false, false};
    actualSameCards.clear();
    firstVec = firstHand.handToVector();
    finalVec = finalHand.handToVector();

    for (qint32 cardIdx = 0; cardIdx < firstVec.size(); ++cardIdx) {
        if (firstVec[cardIdx] == finalVec[cardIdx]) {
            actualSameCards.push_back(true);
        } else {
            actualSameCards.push_back(false);
        }
    }

    QCOMPARE(expectedSameCards, actualSameCards);
}

void JacksOrBetter_OrcTest::testSingleHandInsufficientFunds()
{
    // Setup a game with only 2 credits in the account, then try and play 5
    Account playerAcct;
    playerAcct.add(2);
    JacksOrBetter gameJOB;
    GameOrchestrator orcJOB(gameJOB, 1, playerAcct);
    orcJOB.setCreditsToBet(5);
    orcJOB.dealDraw();
    Hand emptyHand;
    Hand firstHand = orcJOB.retrieveHand(0);
    QVector<PlayingCard> emptyHandVec = emptyHand.handToVector();
    QVector<PlayingCard> firstHandVec = firstHand.handToVector();
    QCOMPARE(emptyHandVec, firstHandVec);

    // Now try and only play 1 credit, there should be a hand after and thus not empty
    orcJOB.setCreditsToBet(1);
    orcJOB.dealDraw();
    firstHand = orcJOB.retrieveHand(0);
    firstHandVec = firstHand.handToVector();
    QVERIFY(emptyHandVec != firstHandVec);
}

void JacksOrBetter_OrcTest::testBetCycling()
{
    Account playerAcct;
    playerAcct.add(100);
    JacksOrBetter gameJOB;
    GameOrchestrator orcJOB(gameJOB, 1, playerAcct);

    // Any PokerGame sub-class initialization (JacksOrBetter in this case) will be for credits-per-bet of 1
    QCOMPARE(orcJOB.creditsToBet(), 1);

    // Cycle through 5 'presses' of the bet increment button to verify: 1->2->3->4->5->1
    orcJOB.cycleBetAmount();
    QCOMPARE(orcJOB.creditsToBet(), 2);
    orcJOB.cycleBetAmount();
    QCOMPARE(orcJOB.creditsToBet(), 3);
    orcJOB.cycleBetAmount();
    QCOMPARE(orcJOB.creditsToBet(), 4);
    orcJOB.cycleBetAmount();
    QCOMPARE(orcJOB.creditsToBet(), 5);
    orcJOB.cycleBetAmount();
    QCOMPARE(orcJOB.creditsToBet(), 1);
}

void JacksOrBetter_OrcTest::testBetMaximum()
{
    Account playerAcct;
    playerAcct.add(100);
    JacksOrBetter gameJOB;
    GameOrchestrator orcJOB(gameJOB, 1, playerAcct);

    // Any PokerGame sub-class initialization (JacksOrBetter in this case) will be for credits-per-bet of 1
    QCOMPARE(orcJOB.creditsToBet(), 1);

    // Call the bet maximum and it should now read 5
    orcJOB.betMaximum();
    QCOMPARE(orcJOB.creditsToBet(), 5);

    // Calling it again should have no impact
    orcJOB.betMaximum();
    QCOMPARE(orcJOB.creditsToBet(), 5);
}

void JacksOrBetter_OrcTest::testBetPlayLose()
{
    Account playerAcct;
    playerAcct.add(100);
    JacksOrBetter gameJOB;
    Hand losingHand(PlayingCard(PlayingCard::CLUB,    PlayingCard::ACE),
                    PlayingCard(PlayingCard::CLUB,    PlayingCard::TWO),
                    PlayingCard(PlayingCard::SPADE,   PlayingCard::FOUR),
                    PlayingCard(PlayingCard::DIAMOND, PlayingCard::NINE),
                    PlayingCard(PlayingCard::HEART,   PlayingCard::SEVEN));
    GameOrchestrator orcJOB(gameJOB, losingHand, playerAcct);

    // Stated with 100, bet 5, lost the hand (no wins), new balance should be 95
    // NOTE: Using the GameOrchestrator constructor above, hold status need not be set (it was done for us)
    orcJOB.setCreditsToBet(5);
    orcJOB.dealDraw();
    orcJOB.dealDraw();

    // Ensure that the hand remained the same
    QVector<PlayingCard> firstHand = losingHand.handToVector();
    QVector<PlayingCard> finalHand = orcJOB.retrieveHand(0).handToVector();
    QCOMPARE(firstHand, finalHand);
    QCOMPARE(playerAcct.balance(), 95);
}

void JacksOrBetter_OrcTest::testBetPlayWinTwoPair()
{
    Account playerAcct;
    playerAcct.add(100);
    JacksOrBetter gameJOB;
    Hand losingHand(PlayingCard(PlayingCard::CLUB, PlayingCard::ACE),
                    PlayingCard(PlayingCard::CLUB, PlayingCard::KING),
                    PlayingCard(PlayingCard::CLUB, PlayingCard::JACK),
                    PlayingCard(PlayingCard::CLUB, PlayingCard::QUEEN),
                    PlayingCard(PlayingCard::CLUB, PlayingCard::TEN));
    GameOrchestrator orcJOB(gameJOB, losingHand, playerAcct);

    // Stated with 100, bet 5, and get the royal flush, new balance should be 4095
    // NOTE: Using the GameOrchestrator constructor above, hold status need not be set (it was done for us)
    orcJOB.setCreditsToBet(5);
    orcJOB.dealDraw();
    orcJOB.dealDraw();
    QCOMPARE(playerAcct.balance(), 4095);
}
