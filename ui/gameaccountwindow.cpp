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
    connect(&_playerAccount, &Account::balanceChanged, this, &GameAccountWindow::updateAccountBalance);
    connect(ui->resetAcct0, &QPushButton::clicked, this, [=]() {
        _playerAccount.setBalance(0);
    });
    connect(ui->addAcct10, &QPushButton::clicked,  this, [=]() {
        _playerAccount.add(10);
    });
    connect(ui->addAcct100, &QPushButton::clicked, this, [=]() {
        _playerAccount.add(100);
    });

    // TODO:
    // For now, do not allow more or less than 1 hand at a time. The Orchestrator UI cannot handle it even though the
    // orchestrator back-end is more than happy to
    ui->handsToPlayLCD->display(1);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * ALL SUPPORTED GAMES SHOULD BE PUSHED HERE SO THEY ARE RENDERED TO THE ACCOUNT / GAME SELECTION SCREEN         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    _supportedGames.push_back(new JacksOrBetter);
    _supportedGames.push_back(new BonusPoker);
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    // Loop over all games added above and create buttons + connections to launch them
    for (int gameIdx = 0; gameIdx < _supportedGames.size(); ++gameIdx) {
        QString nameOfGame = _supportedGames[gameIdx]->gameName();
        QPushButton *gameStartButton = new QPushButton(nameOfGame, this);
        ui->gameSelectFrame->layout()->addWidget(gameStartButton);

        // Connect the button to the game starter
        connect(gameStartButton, &QPushButton::clicked,
                this, [=]() {
            startGame(_supportedGames[gameIdx]);
        });
    }

    // When games are added they will mess up the intrinsic "tab" ordering of the main window, so build a new ordering
    // First the credits-available:
    setTabOrder(ui->addAcct100, ui->addAcct10);
    setTabOrder(ui->addAcct10, ui->resetAcct0);

    int i;
    for (i = 0; i < ui->gameSelectFrame->layout()->count(); ++i) {
        if (i == 0) {
            setTabOrder(ui->resetAcct0, ui->gameSelectFrame->layout()->itemAt(i)->widget());
        } else {
            setTabOrder(ui->gameSelectFrame->layout()->itemAt(i - 1)->widget(),
                        ui->gameSelectFrame->layout()->itemAt(i)->widget());
        }
    }

    // Then finally link the Exit and About... Buttons
    setTabOrder(ui->gameSelectFrame->layout()->itemAt(i - 1)->widget(), ui->exitButton);
    setTabOrder(ui->exitButton, ui->aboutButton);

    // SOFTKEY ASSIGNMENT
    // Move Down / Up:
    connect(ui->moveDnSoftkey, &QPushButton::clicked, this, &GameAccountWindow::focusNextChild);
    connect(ui->moveUpSoftkey, &QPushButton::clicked, this, &GameAccountWindow::focusPreviousChild);

    // Select
    connect(ui->selectSoftkey, &QPushButton::clicked, this, [=]() {
        // Hope this doesn't crash ... anyway the user will only be able to cycle through pushbuttons
        QPushButton *currentlyFocused = dynamic_cast<QPushButton*>(this->focusWidget());
        currentlyFocused->click();
    });

    // Number of Hands Increment / Decrement
    connect(ui->nbHandsDec, &QPushButton::clicked, this, [=]() {changeNumberOfHands(-1);});
    connect(ui->nbHandsInc, &QPushButton::clicked, this, [=]() {changeNumberOfHands( 1);});

    // Uncomment to make window full-screen
//    this->setWindowState(this->windowState() | Qt::WindowFullScreen);

    // Uncomment the line below to mask the mouse pointer
//    QApplication::setOverrideCursor(Qt::BlankCursor);
}

GameAccountWindow::~GameAccountWindow()
{
    delete ui;
}

void GameAccountWindow::updateAccountBalance(quint32 updatedBalance)
{
    // WARNING: Static cast could yeild strangeness for big numbers of credits
    ui->creditCountLCD->display(static_cast<int>(updatedBalance));
}

void GameAccountWindow::startGame(PokerGame *gameLogicPointer)
{
    // How many simultaneous hands are played per betting round?
    int numberOfHands = ui->handsToPlayLCD->value();

    // Create a game and link it to this parent, open the window
    GameOrchestratorWindow *gow = new GameOrchestratorWindow(_playerAccount, gameLogicPointer, numberOfHands, this);

    // Needed so that memory is automatically cleaned out when closing the window!
    gow->setAttribute(Qt::WA_DeleteOnClose);

    // Start the game
    gow->show();
}

void GameAccountWindow::changeNumberOfHands(int increaseDecrease)
{
    int currentHandCount = ui->handsToPlayLCD->value();
    int nextHandCount    = 1;

    // Because of rendering constraints, adjust the number of hands not 1-by-1 but with specific values
    if (increaseDecrease == -1) {
        switch (currentHandCount) {
        case 1:
//            nextHandCount = 100;
            nextHandCount = 25;
            break;
        case 5:
            nextHandCount = 3;
            break;
        case 10:
            nextHandCount = 5;
            break;
        case 25:
            nextHandCount = 10;
            break;
//        case 100:
//            nextHandCount = 25;
//            break;
        default:
            nextHandCount = 1;
        }
    } else if (increaseDecrease == 1) {
        switch (currentHandCount) {
        case 1:
            nextHandCount = 3;
            break;
        case 3:
            nextHandCount = 5;
            break;
        case 5:
            nextHandCount = 10;
            break;
        case 10:
            nextHandCount = 25;
            break;
//        case 25:
//            nextHandCount = 100;
//            break;
        default:
            nextHandCount = 1;
        }
    }

    // Update the UI control with the new value
    ui->handsToPlayLCD->display(nextHandCount);
}

