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
#include "jacksorbetter.h"
#include "account.h"

namespace Ui {
class GameOrchestratorWindow;
}

class GameOrchestratorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameOrchestratorWindow(Account &playerAccount, QWidget *parent = nullptr);
    ~GameOrchestratorWindow();

public slots:
    // Paytable updates when the betting is changed
    void showPayTableAndBet();

    // set showDraw to true so the button for deal/draw only says "Draw", otherwise it is "Deal"
    void dealToDraw(bool showDraw);

    // Update winnings as hand(s) processed by the orchestrator
    void currentWinnings(quint32 winningsOfHands);

    // Update the account balance widget as the totals are created
    void currentBalance(quint32 totalCredits);

    // TODO: This is terrible ... I should be able to make a single function to do this?
    void holdCard1(bool cardHeld);
    void holdCard2(bool cardHeld);
    void holdCard3(bool cardHeld);
    void holdCard4(bool cardHeld);
    void holdCard5(bool cardHeld);

private:
    Account          &_playerCredits;
    GameOrchestrator *_gameOrc;
    JacksOrBetter    *_gameLogic;
    QThread          *_gameEventProcessor;     // TODO: Figure out threading for worker process
    Ui::GameOrchestratorWindow *ui;
};

#endif // GAMEORCHESTRATORWINDOW_H
