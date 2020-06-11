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

#include "gameaccountwindow.h"
#include "ui_gameaccountwindow.h"

// Game Orchestrator Window can be started with all different supported PokerGame subclasses
#include "gameorchestratorwindow.h"

// Individual games supported (sub-classed from PokerGame)
#include "jacksorbetter.h"
#include "bonuspoker.h"

GameAccountWindow::GameAccountWindow(QWidget *parent)
    : QMainWindow(parent)
    , _playerAccount()
    , ui(new Ui::GameAccountWindow)
{
    ui->setupUi(this);

    // Connect the widgets initialized to their appropriate slots
    connect(ui->exitButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);

    // TODO: Make an "About..." window ...

    // Connect the value of the spinbox to the actual account balance + support buttons for changing values
    connect(ui->accountBalance, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GameAccountWindow::setAccountBalance);
    connect(ui->resetAcct0, &QPushButton::clicked, this, [=]() {
        ui->accountBalance->setValue(0);
    });
    connect(ui->addAcct10, &QPushButton::clicked,  this, [=]() {
        ui->accountBalance->setValue(ui->accountBalance->value() + 10);
    });
    connect(ui->addAcct100, &QPushButton::clicked, this, [=]() {
        ui->accountBalance->setValue(ui->accountBalance->value() + 100);
    });

    ;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * ALL SUPPORTED GAMES SHOULD BE PUSHED HERE SO THEY ARE RENDERED TO THE ACCOUNT / GAME SELECTION SCREEN         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    _supportedGames.push_back(new JacksOrBetter);
    _supportedGames.push_back(new BonusPoker);
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    // Loop over all games added above and create buttons + connections to launch them
    int gameIdx;
    for (gameIdx = 0; gameIdx < _supportedGames.size(); ++gameIdx) {
        QString nameOfGame = _supportedGames[gameIdx]->gameName();
        QPushButton *gameStartButton = new QPushButton(nameOfGame, this);
        QSpinBox *nbSimulHandsSelect = new QSpinBox(this);

        // For now, we can only support 1 hand games (UI is hardcoded against this, but the orchestrator supports >= 1)
        nbSimulHandsSelect->setMinimum(1);
        nbSimulHandsSelect->setMaximum(1);

        ui->gameSelectFrame->layout()->addWidget(gameStartButton/*, gameIdx, 0*/);
        ui->gameSelectFrame->layout()->addWidget(nbSimulHandsSelect/*, gameIdx, 1*/);

        // Connect the button to the game starter
        connect(gameStartButton, &QPushButton::clicked,
                this, [=]() {
            startGame(_supportedGames[gameIdx], nbSimulHandsSelect->value());
        });
    }
}

GameAccountWindow::~GameAccountWindow()
{
    delete ui;
}

void GameAccountWindow::setAccountBalance()
{
    _playerAccount.setBalance(ui->accountBalance->value());
}

void GameAccountWindow::startGame(PokerGame *gameLogicPointer, int numberOfHands)
{
    // Create a game and link it to this parent, open the window
    GameOrchestratorWindow *gow = new GameOrchestratorWindow(_playerAccount, gameLogicPointer, numberOfHands, this);

    // Needed so that memory is automatically cleaned out when closing the window!
    gow->setAttribute(Qt::WA_DeleteOnClose);

    // Start the game
    gow->show();
}

