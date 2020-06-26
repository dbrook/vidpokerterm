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

#include "gameaccountinterface.h"

#include "gameorchestratorinterface.h"

// Supported Poker Games
#include "jacksorbetter.h"
#include "bonuspoker.h"

#include <QThread>
#include <QDebug>

GameAccountInterface::GameAccountInterface(int                  nbSoftkeys,
                                           GenericLCD          *lcdScreen,
                                           GenericInputHandler *inputs,
                                           bool                *haltFlag,
                                           QObject             *parent)
    : LCDInterface    (nbSoftkeys, parent),
      _lcd            (lcdScreen),
      _input          (inputs),
      _haltFlag       (haltFlag),
      _selectedGameIdx(0)
{
    // Fill in all the softkey functions
    this->addSoftkeyFunction("CRD+", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::addCredits));
    this->addSoftkeyFunction("CRD0", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::zeroCredits));
    this->addSoftkeyFunction("GAME", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::cycleGame));
    this->addSoftkeyFunction("HALT", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::exitApplication));
//    this->addSoftkeyFunction("FNC0", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC1", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC2", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC3", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC4", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC5", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
//    this->addSoftkeyFunction("FNC6", static_cast<void (LCDInterface::*)()>(&GameAccountInterface::noopSoftkeyItem));
    this->finishSoftkeys();

    // A player needs to have funds with which to gamble
    _playerCreds = new Account(this);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * ALL SUPPORTED GAMES SHOULD BE PUSHED HERE SO THEY ARE RENDERED TO THE ACCOUNT / GAME SELECTION SCREEN         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    _supportedGames.push_back(new JacksOrBetter);
    _supportedGames.push_back(new BonusPoker);
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    restoreConnections();
    _playerCreds->setBalance(0);
    this->softkeys();
}

GameAccountInterface::~GameAccountInterface()
{
    // Clean up the games
    for (PokerGame *game : _supportedGames) {
        delete game;
    }
}

void GameAccountInterface::exitApplication()
{
    qDebug() << "Halting the application!";
    *_haltFlag = true;
    emit haltRequested();
}

void GameAccountInterface::addCredits()
{
    _playerCreds->add(100);
}

void GameAccountInterface::zeroCredits()
{
    _playerCreds->setBalance(0);
}

void GameAccountInterface::cycleGame()
{
    _selectedGameIdx++;
    if (_selectedGameIdx >= _supportedGames.size()) {
        _selectedGameIdx = 0;
    }
    emit selectedGame(_supportedGames[_selectedGameIdx]->gameName());
}

void GameAccountInterface::playSelectedGame()
{
    // Suspend the connections from the interface to this screen
    disconnect(this, &GameAccountInterface::softkeyAtPosition, _lcd, &GenericLCD::fillSoftkeyText);
    disconnect(_input, &GenericInputHandler::softkeyPressed, this, &GameAccountInterface::triggerSoftkey);
    disconnect(_playerCreds, &Account::balanceChanged, _lcd, &GenericLCD::showCreditsInMainWin);
    disconnect(this, &GameAccountInterface::selectedGame, _lcd, &GenericLCD::showGameName);
    disconnect(_input, &GenericInputHandler::triggerPressed, this, &GameAccountInterface::playSelectedGame);

    // Call the game
    GameOrchestratorInterface *gameToPlay = new GameOrchestratorInterface(_nbSoftkeys,
                                                                          _lcd,
                                                                          _input,
                                                                          _playerCreds,
                                                                          _supportedGames[_selectedGameIdx]);
    QThread *orcThread = new QThread();
    gameToPlay->moveToThread(orcThread);
    orcThread->start();

    // When control returns ... restore the input processor here
    connect(gameToPlay, &GameOrchestratorInterface::gameCompleted, gameToPlay, &GameOrchestratorInterface::deleteLater);
    connect(gameToPlay, &GameOrchestratorInterface::destroyed, this, &GameAccountInterface::restoreConnections);
    connect(gameToPlay, &GameOrchestratorInterface::destroyed, orcThread, &QThread::quit);
}

void GameAccountInterface::restoreConnections()
{
    connect(this, &GameAccountInterface::softkeyAtPosition, _lcd, &GenericLCD::fillSoftkeyText);
    connect(_input, &GenericInputHandler::softkeyPressed, this, &GameAccountInterface::triggerSoftkey);
    connect(_playerCreds, &Account::balanceChanged, _lcd, &GenericLCD::showCreditsInMainWin);
    connect(this, &GameAccountInterface::selectedGame, _lcd, &GenericLCD::showGameName);
    connect(_input, &GenericInputHandler::triggerPressed, this, &GameAccountInterface::playSelectedGame);

    // Reprint everything?
    _lcd->setupWelcomeDisplay();   // TODO: might need to make this thread safe?
    this->softkeys();
    emit selectedGame(_supportedGames[_selectedGameIdx]->gameName());
    _playerCreds->setBalance(_playerCreds->balance());
}

