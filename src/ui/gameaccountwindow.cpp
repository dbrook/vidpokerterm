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

GameAccountWindow::GameAccountWindow(QWidget *parent)
    : QMainWindow(parent)
    , _playerAccount()
    , ui(new Ui::GameAccountWindow)
{
    ui->setupUi(this);

    // Connect the widgets initialized to their appropriate slots
    connect(ui->exitButton, &QPushButton::clicked,
            QApplication::instance(), &QApplication::quit);

    // TODO: Make an "About..." window ...

    // TODO: Make it so account can be updated using the widgets created ...
    connect(ui->accountBalance, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GameAccountWindow::setAccountBalance);

    // Start Jacks Or Better
    // TODO: This should be filled in for each game supported, but for now it's hardcoded on the UI side
    connect(ui->jobTestBtn, &QPushButton::clicked,
            this, &GameAccountWindow::startGame);
}

GameAccountWindow::~GameAccountWindow()
{
    delete ui;
}

void GameAccountWindow::setAccountBalance()
{
    _playerAccount.setBalance(ui->accountBalance->value());
}

void GameAccountWindow::startGame()
{
    // Create a game and link it to this parent, open the window
    GameOrchestratorWindow *gow = new GameOrchestratorWindow(_playerAccount, this);

    // Needed so that memory is automatically cleaned out when closing the window!
    gow->setAttribute(Qt::WA_DeleteOnClose);

    // Start the game
    gow->show();
}

