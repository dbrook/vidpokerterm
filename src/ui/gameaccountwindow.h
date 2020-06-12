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

#ifndef GAMEACCOUNTWINDOW_H
#define GAMEACCOUNTWINDOW_H

#include <QMainWindow>

// The player's account is maintained in this window
#include "account.h"

// Generic PokerGame logic
#include "pokergame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameAccountWindow; }
QT_END_NAMESPACE

class GameAccountWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameAccountWindow(QWidget *parent = nullptr);
    ~GameAccountWindow();

public slots:
    /**
     * @brief updateAccountBalance will reflect the balance of credits in a player's account in a LCD display widget
     */
    void updateAccountBalance(quint32 updatedBalance);

    /**
     * @brief startGame begins a game of whatever PokerGame is pointed to
     */
    void startGame(PokerGame *gameLogicPointer);

    /**
     * @brief      changeNumberOfHands sets the number of hands a player will play at a time in the game orchestrator
     *
     * @param[in]  increaseDecrease use -1 to decrease the hands played, 1 to increase it
     *
     * @note       The minimum value held is 1, the maximum value is ---TODO--- (after UI supports multi-hands)
     */
    void changeNumberOfHands(int increaseDecrease);

private:
    // Game-Specific Items
    Account _playerAccount;

    // List of playable games -- vector of generic "PokerGame" classes, to be filled with pointers to actual logic
    QVector<PokerGame *> _supportedGames;

    // User Interface Items
    Ui::GameAccountWindow *ui;
};
#endif // GAMEACCOUNTWINDOW_H
