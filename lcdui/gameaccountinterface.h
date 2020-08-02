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

#ifndef GAMEACCOUNTINTERFACE_H
#define GAMEACCOUNTINTERFACE_H

#include "lcdinterface.h"

// Low-Level Interfaces (inputs/screen)
#include "genericlcd.h"
#include "genericinputhandler.h"

// VidPokerTerm library
#include "account.h"
#include "pokergame.h"

class GameAccountInterface : public LCDInterface
{
    Q_OBJECT
public:
    explicit GameAccountInterface(int                  nbSoftkeys,
                                  GenericLCD          *lcdScreen,
                                  GenericInputHandler *inputs,
                                  bool                *haltFlag,
                                  QObject             *parent = nullptr);

    ~GameAccountInterface();

public slots:
    /**
     * @brief exitApplication will call the QCoreApplication shutdown procedure
     */
    void exitApplication();

    /**
     * @brief addCredits will add 100 credits to the attached account
     */
    void addCredits();

    /**
     * @brief zeroCredits will reset the credit count to 0
     */
    void zeroCredits();

    /**
     * @brief cycleGame iterates through the games in the _supportedGames vector
     */
    void cycleGame();

    /**
     * @brief playSelectedGame will start a game orchestrator screen with the currently-selected game logic
     */
    void playSelectedGame();

    /**
     * @brief restoreConnections sets all the input events to the appropriate targets
     */
    void restoreConnections();

signals:
    /**
     * @brief haltRequested is emitted when the player has selected the quit option
     */
    void haltRequested();

    /**
     * @brief resetDisplay is emitted when the display should be fully reconditioned
     */
    void resetDisplay();

    /**
     * @brief selectedGame is emitted when a game selection change is made
     *
     * @param nameOfGame          The name of the game currently selected for opening
     */
    void selectedGame(const QString &nameOfGame);

private:
    // Memory managed outside of this class
    GenericLCD          *_lcd;
    GenericInputHandler *_input;
    bool                *_haltFlag;

    // Memory managed by this class
    Account             *_playerCreds;

    // List of games a player may select
    QVector<PokerGame *> _supportedGames;
    int                  _selectedGameIdx;
};

#endif // GAMEACCOUNTINTERFACE_H
