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

#ifndef GAMEORCHESTRATORWINDOW_H
#define GAMEORCHESTRATORWINDOW_H

#include <QMainWindow>

// Do not block the main UI while game logic is running
#include <QThread>

// A game orchestrator supports an abstract PokerGame instance and account
#include "gameorchestrator.h"
#include "account.h"

namespace Ui {
class GameOrchestratorWindow;
}

class HandWidget;

/**
 * @brief The GameOrchestratorWindow provides a graphical interface to a generic PokerGame logic via an orchestrator
 *        which handles dealing, holding, drawing, betting, winning operations with an Account which tracks all funds.
 *
 * @note  Multiple-Hand Support
 *        The orchestrator can play as many hands at a time as a player wishes to wager. The player interacts with a
 *        primary hand which renders at deal time. Only this hannd can have card held, it will hold the same card on
 *        all other hands too. Then at draw time, all the hands will be filled with their shuffled decks.
 *        Constraints about realistic betting amounts and how many hands can fit on a screen mean an upper limit of
 *        100 simultaneous hands. The way these "secondary hands" are drawn to the screen depends on how many are
 *        played. The increment is not uniform either, but stepped: 1 -> 3 -> 5 -> 10 -> 25 ---> 100 (!)
 *
 *        Be advised that screen constraints get a little troublesome above 10 hands, especially on 1080p monitors.
 *
 *        In the constructor, the font and pixel sizes can be updated to work with your screen situation (since this
 *        was originally designed for desktop monitors and then kludged into smaller screen resolutions).
 */
class GameOrchestratorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameOrchestratorWindow(Account   &playerAccount,
                                    PokerGame *gameLogic,
                                    int        handsToPlay,
                                    QWidget   *parent = nullptr);

    ~GameOrchestratorWindow();

    // For custom resize handling of widgets
    // TODO: these could change depending on how the rendering of the paytable could be better handled
    //       (they also do not appear to work when the game window is initially spawned, only on resizes)
    void resizeEvent(QResizeEvent* event);
    void fixPayoutTableSize();

public slots:
    // Paytable updates when the betting is changed
    void showPayTableAndBet();

    // set showDraw to true so the button for deal/draw only says "Draw", otherwise it is "Deal"
    void dealToDraw(bool showDraw);

    // Disables the deal-draw button so it cannot be operated
    void disableDealDraw(bool dealDrawDisabled);

    // Resets the cards BEFORE calling the dealDraw of the orchestrator (prevents exceptions/collisions between threads)
    void syncDealDraw();

    // Update winnings as hand(s) processed by the orchestrator
    void currentWinnings(quint32 winningsOfHands);

    // Update the account balance widget as the totals are created
    void currentBalance(quint32 totalCredits);

    // Sets the render speed indicator on the speed button
    void updateSpeedChar(const QString &speedStr);

    // TODO: This is terrible ... I should be able to make a single function to do this?
    void holdCard1(bool cardHeld);
    void holdCard2(bool cardHeld);
    void holdCard3(bool cardHeld);
    void holdCard4(bool cardHeld);
    void holdCard5(bool cardHeld);

    // Update Secondary Hand at secoHandPos
    void updateSecondaryHandCard(int secoHandPos, int cardPos, PlayingCard cardToShow, bool show);

    // Show the win of a secondary hand at secoHandPos
    void secondaryWinTextAndAmt(int secoHandPos, const QString &handString, quint32 winning);

    // Flips over all cards of all hands
    void flipAllHands();

signals:
    // Should be emitted before calling the dealDraw to give the UI time to catch up before the orchestrator delivers
    // any new cards
    void resetCardDisplay();

    // Emitted when the window is ready for the orchestrator to deliver cards
    void readyForDealDraw();

private:
    Account              &_playerCredits;
    GameOrchestrator     *_gameOrc;
    PokerGame            *_gameLogic;
    int                   _handsToPlay;
    QVector<HandWidget*>  _addedHands;
    QThread              *_gameEventProcessor;
    Ui::GameOrchestratorWindow *ui;
};

#endif // GAMEORCHESTRATORWINDOW_H
