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

#ifndef GAMEORCHESTRATORINTERFACE_H
#define GAMEORCHESTRATORINTERFACE_H

#include "lcdinterface.h"
#include "genericlcd.h"
#include "genericinputhandler.h"

#include "account.h"
#include "gameorchestrator.h"
#include "pokergame.h"

#include <QObject>

class GameOrchestratorInterface : public LCDInterface
{
    Q_OBJECT
public:
    explicit GameOrchestratorInterface(int                  nbSoftkeys,
                                       GenericLCD          *lcdScreen,
                                       GenericInputHandler *inputs,
                                       Account             *playerCreds,
                                       PokerGame           *gameLogic,
                                       QObject             *parent = nullptr);

    ~GameOrchestratorInterface();

public slots:
    /**
     * @brief closeGame is called to emit that this instance may be deleted and control returned to the previous screen
     */
    void closeGame();

    /**
     * @brief showBetAmount
     */
    void showBetAmount();

    /**
     * @brief betPlus softkey wrapper for orchestrator's bet cycler
     */
    void betPlus();

    /**
     * @brief betMax softkey wrapper for orchestrator's bet maximum
     */
    void betMax();

    /**
     * @brief resetHolds unsets the hold status for interfacing with the orchestrator
     */
    void resetHolds();

    /**
     * @brief toggleHold will set/unset the hold status of a card based on the current status, and emit the new value
     *
     * @param handCardIdx         index of the card to hold (0, 1, 2, 3, 4)
     */
    void toggleHold(int handCardIdx);

    /**
     * @brief allowHolds indicates a game is in progress and cards can be held
     * @param allowed             true if holds can be issued, false if not and the interface should reset
     */
    void allowHolds(bool allowed);

signals:
    void gameCompleted();

    void betAmountUpdated(quint32 totalBetAmount);

    void cardHeld(int cardIdx, bool cardIsHeld);

    void cardHoldIndic(int cardIdx);

private:
    GenericLCD          *_lcd;
    GenericInputHandler *_input;
    Account             *_creds;
    PokerGame           *_game;
    GameOrchestrator    *_synchroOrc;
    bool                 _holdCard1;
    bool                 _holdCard2;
    bool                 _holdCard3;
    bool                 _holdCard4;
    bool                 _holdCard5;

    QVector<QPair<QString, void (LCDInterface::*)()>> _hiddenKeys;
};

#endif // GAMEORCHESTRATORINTERFACE_H
