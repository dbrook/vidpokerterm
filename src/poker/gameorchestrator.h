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

#ifndef GAMEORCHESTRATOR_H
#define GAMEORCHESTRATOR_H

#include "deck.h"
#include "hand.h"
#include "pokergame.h"
#include "account.h"

#include <QObject>
#include <QVector>

class GameOrchestrator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief GameOrchestrator constructor using a generic PokerGame x number of simultaneous hands, and credit account
     *
     * @param[in]  gameAnalyzer    pointer to an instance of an actual PokerGame subclass
     * @param[in]  nbHandsToPlay   number of hands to play simultaneously (usually will be 1)
     * @param[in]  playerAcct      reference to an account for managing the credits available for betting/winning
     * @param[in]  renderDelay     delay between card draws in milliseconds
     * @param[in]  parent          QObject parent pointer
     */
    explicit GameOrchestrator(PokerGame *gameAnalyzer,
                              quint32    nbHandsToPlay,
                              Account   &playerAcct,
                              quint8     renderDelay,
                              QObject   *parent = nullptr);

    /**
     * @brief GameOrchestrator constructor but with a forced input hand for testing that winning amounts are updated
     *
     * @note  this is only for the sake of testing, it should not be used in an actual game orchestration or the player
     *        will become extremely bored seeing the same hand over and over again :)
     *
     * @note  drawing any cards in this flow is not expected, you should keep all cards set as holds (which this
     *        constructor does for you)
     *
     * @param[in]  gameAnalyzer    pointer to an instance of an actual PokerGame subclass
     * @param[in]  fixedHandTest   reference to a hand to play rather than drawing from a random deck
     * @param[in]  playerAcct      reference to an account for managing the credits available for betting/winning
     * @param[in]  parent          QObject parent pointer
     */
    explicit GameOrchestrator(PokerGame *gameAnalyzer,
                              Hand      &fixedHandTest,
                              Account   &playerAcct,
                              QObject   *parent = nullptr);

    /**
     * @brief retrieveHand returns the hand at a requested index so it may be inspected for unit testing
     *
     * @param[in]  handNumber      0-indexed entry in the internal hand vector
     *
     * @exception  runtime_error will be raised if the index is out of bounds ("Requested hand was out of range")
     *
     * @return a single Hand item at the position requested
     */
    Hand retrieveHand(qint32 handNumber) const;

    /**
     * @brief setCreditsToBet forwards the number of credits to apply towards each hand into the gameAnalyzer for the
     *        sake of computing winnings, but also for each deal operation to ensure a player can cover the bet.
     *
     * @param[in]  credits
     */
    void setCreditsToBet(qint32 credits);

    /**
     * @brief creditsToBet fetches how many credits the player must apply to a deal as stored in the gameAnalyzer.
     *        This should be used for the sake of unit testing.
     *
     * @return number of credits a player must have to cover the next deal of the cards
     */
    qint8 creditsToBet() const;

    /**
     * @brief currentPayTable retrieves the currently active pay table and hand names for display purposes
     *
     * @param[out] payTable        reference to an array to be filled with the pay table
     */
    void currentPayTable(QVector<QPair<const QString, int>> &payTable);

    /**
     * @brief isGameInProgress determines if the orchestrator is waiting for the use to hold cards and redraw
     *
     * @return true if there is a hand / game in progress
     */
    bool isGameInProgress() const;

public slots:
    /**
     * @brief dealDraw will deal 5 cards per _gameCard pair when called the first time. When called the second time, it
     *        will only replace the cards that were not held with new cards.
     */
    void dealDraw();

    /**
     * @brief hold ensures a card will not be replaced with a new card from the following call to dealDraw
     *
     * @param[in]  cardPosition    position in the deck to set the hold status
     * @param[in]  canHold         true if the card should not be replaced with a call to dealDraw
     */
    void hold(quint8 cardPosition, bool canHold);

    /**
     * @brief cycleBetAmount allows somebody to press the bet button to step how much they will bet per deal, like so:
     *        Bet 1 --> Bet 2 --> Bet 3 --> Bet 4 --> Bet 5 --+
     *          ^                                             |
     *          +---------------------------------------------+
     */
    void cycleBetAmount();

    /**
     * @brief betMaximum does just what it says, jump straight to the maximum bet per deal
     */
    void betMaximum();

    /**
     * @brief speedControl will adjust the delay between card draws:
     *
     *            +--> Instant --> Slow 150ms --> Medium 100ms --> Fast 50ms ---+
     *            |                                                             |
     *            +-------------------------------------------------------------+
     */
    void speedControlCycle();

signals:
    /**
     * @brief betUpdated is emitted when the user requested a bet amount change and a new paytable should be shown
     */
    void betUpdated();

    /**
     * @brief cardsToRedraw is emitted with the status (true) of cards that will be redrawn (for the sake of the UI)
     */
    void cardsToRedraw(bool card1, bool card2, bool card3, bool card4, bool card5);

    /**
     * @brief gameInProgress indicates a game is in progress and the next dealDraw call will be a draw, not a full deal
     */
    void gameInProgress(bool sayDrawNotDeal);

    /**
     * @brief primaryHandUpdated indicates the result of the hand analysis and any winnings associated with that hand
     */
    void primaryHandUpdated(const QString &handString, quint32 winning);

    /**
     * @brief gameWinnings indicates how much has been won on the game so far (running total of all hands played)
     */
    void gameWinnings(qint32 winningsSoFar);

    /**
     * @brief primaryCardRevealed indicates a card on the primary hand was revealed (at index and what the card was)
     */
    void primaryCardRevealed(int cardIdx, PlayingCard card);

    /**
     * @brief renderSpeed indicates the card draw speed was changed
     */
    void renderSpeed(const QString &currentSpeed);

private:
    PokerGame                  *_gameAnalyzer;
    quint32                     _nbHandsToPlay;
    quint32                     _betsPerHand;
    Account                    &_playerAccount;
    quint8                      _renderDelayMS;
    bool                        _fakeGame;
    bool                        _handInProg;
    QVector<QPair<Deck, Hand>>  _gameCards;
};

#endif // GAMEORCHESTRATOR_H
