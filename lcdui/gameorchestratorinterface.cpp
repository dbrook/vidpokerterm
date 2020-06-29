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

#include "gameorchestratorinterface.h"

#include <QDebug>

GameOrchestratorInterface::GameOrchestratorInterface(int                  nbSoftkeys,
                                                     GenericLCD          *lcdScreen,
                                                     GenericInputHandler *inputs,
                                                     Account             *playerCreds,
                                                     PokerGame           *gameLogic,
                                                     QObject             *parent)
    : LCDInterface(nbSoftkeys, parent),
      _lcd        (lcdScreen),
      _input      (inputs),
      _creds      (playerCreds),
      _game       (gameLogic)
{
    qRegisterMetaType<PlayingCard>("PlayingCard");
    _synchroOrc = new GameOrchestrator(_game, 1, *_creds, 0);

    // Initialize the game display
    _lcd->setupGameDisplay();

    // Reset / initialize the holds
    this->resetHolds();

    // Connect all the events up
    connect(_creds, &Account::balanceChanged, _lcd, &GenericLCD::showCreditsInGame);
    connect(this, &GameOrchestratorInterface::softkeysForPage, _lcd, &GenericLCD::fillSoftkeys);
    connect(_input, &GenericInputHandler::softkeyPressed, this, &GameOrchestratorInterface::triggerSoftkey);
    connect(_synchroOrc, &GameOrchestrator::betUpdated, this, &GameOrchestratorInterface::showBetAmount);
    connect(this, &GameOrchestratorInterface::betAmountUpdated, _lcd, &GenericLCD::showBetAmount);
    connect(_input, &GenericInputHandler::triggerPressed, _synchroOrc, &GameOrchestrator::dealDraw);
    connect(_synchroOrc, &GameOrchestrator::primaryCardRevealed, _lcd, &GenericLCD::showCardValue);
    connect(this, &GameOrchestratorInterface::cardHeld, _lcd, &GenericLCD::showHoldIndicator);
    connect(this, &GameOrchestratorInterface::holdsReset, _lcd, &GenericLCD::clearAllHolds);
    connect(_synchroOrc, &GameOrchestrator::readyForHolds, this, &GameOrchestratorInterface::allowHolds);
    connect(_synchroOrc, &GameOrchestrator::cardsToRedraw, _lcd, &GenericLCD::showCardFrames);
    connect(_synchroOrc, &GameOrchestrator::primaryHandUpdated, _lcd, &GenericLCD::showWinnings);
    connect(_synchroOrc, &GameOrchestrator::insufficientFunds, _lcd, &GenericLCD::displayNoFundsWarning);

    // Setup the softkey functions and broadcast them to the display
    this->addSoftkeyFunction("Return", static_cast<void (LCDInterface::*)()>(&GameOrchestratorInterface::closeGame));
    this->addSoftkeyFunction("Bet +1", static_cast<void (LCDInterface::*)()>(&GameOrchestratorInterface::betPlus));
    this->addSoftkeyFunction("BetMax", static_cast<void (LCDInterface::*)()>(&GameOrchestratorInterface::betMax));
    this->finishSoftkeys();
    this->softkeyPage();

    // Trigger the display of the balance by changing it to itself
    _creds->setBalance(_creds->balance());

    // Trigger the display of the bet to the display
    _synchroOrc->setCreditsToBet(1);
}

GameOrchestratorInterface::~GameOrchestratorInterface()
{
    delete _synchroOrc;
    disconnect(_creds, &Account::balanceChanged, _lcd, &GenericLCD::showCreditsInGame);
    qDebug() << "Closing the game display";
}

void GameOrchestratorInterface::closeGame()
{
    emit gameCompleted();
}

void GameOrchestratorInterface::showBetAmount()
{
    emit betAmountUpdated(_synchroOrc->creditsToBet());
}

void GameOrchestratorInterface::betPlus()
{
    _synchroOrc->cycleBetAmount();
}

void GameOrchestratorInterface::betMax()
{
    _synchroOrc->betMaximum();
}

void GameOrchestratorInterface::resetHolds()
{
    _holdCard1 = false;
    _holdCard2 = false;
    _holdCard3 = false;
    _holdCard4 = false;
    _holdCard5 = false;
    emit holdsReset();
}

void GameOrchestratorInterface::toggleHold(int handCardIdx)
{
    switch (handCardIdx) {
    case 0:
        _holdCard1 = !_holdCard1;
        emit cardHeld(0, _holdCard1);
        break;
    case 1:
        _holdCard2 = !_holdCard2;
        emit cardHeld(1, _holdCard2);
        break;
    case 2:
        _holdCard3 = !_holdCard3;
        emit cardHeld(2, _holdCard3);
        break;
    case 3:
        _holdCard4 = !_holdCard4;
        emit cardHeld(3, _holdCard4);
        break;
    case 4:
        _holdCard5 = !_holdCard5;
        emit cardHeld(4, _holdCard5);
        break;
    default:
        ;
    }
}

void GameOrchestratorInterface::allowHolds(bool allowed)
{
    // Swap the softkey buffer the softkeys
    QVector<QPair<QString, void (LCDInterface::*)()>> swap = _hiddenKeys;
    _hiddenKeys  = _softkeys;
    _softkeys    = swap;
    _softkeyPage = 0;

    // The first time the keys are disabled, the "empty" ones need to be finalized to render properly
    if (_softkeys.empty()) {
        this->finishSoftkeys();
    }

    this->softkeyPage();

    if (allowed) {
        connect(_input, &GenericInputHandler::holdPressed, this, &GameOrchestratorInterface::toggleHold);
        connect(this, &GameOrchestratorInterface::cardHeld, _synchroOrc, &GameOrchestrator::hold);
    } else {
        disconnect(_input, &GenericInputHandler::holdPressed, this, &GameOrchestratorInterface::toggleHold);
        disconnect(this, &GameOrchestratorInterface::cardHeld, _synchroOrc, &GameOrchestrator::hold);
        this->resetHolds();
    }
}
