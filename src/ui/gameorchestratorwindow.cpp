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

#include "gameorchestratorwindow.h"
#include "ui_gameorchestratorwindow.h"

// Hand widget for showing cards, hold status, etc.
#include "handwidget.h"

// Individual games supported
#include "jacksorbetter.h"

GameOrchestratorWindow::GameOrchestratorWindow(Account &playerAccount, QWidget *parent) :
    QMainWindow(parent),
    _playerCredits(playerAccount),
    ui(new Ui::GameOrchestratorWindow)
{
    // Setup the UI
    ui->setupUi(this);

    // Now bring in the game logic
    _gameLogic = new JacksOrBetter;
    _gameOrc   = new GameOrchestrator(_gameLogic, 1, _playerCredits, 0);

    // Fill in the number of credits the first time
    ui->creditsAmount->display(static_cast<int>(_playerCredits.balance()));

    // Fill the game name string in the table and the main window title
    setWindowTitle(_gameLogic->gameName());
    ui->gameLabel->setText(_gameLogic->gameName());

    // Connect the paytable display to all changes made to the betting amount
    connect(_gameOrc, &GameOrchestrator::betUpdated, this, &GameOrchestratorWindow::showPayTableAndBet);

    // Connect betting buttons to the game orchestrator
    connect(ui->betIncrementButton, &QPushButton::clicked, _gameOrc, &GameOrchestrator::cycleBetAmount);
    connect(ui->maxBetButton, &QPushButton::clicked, _gameOrc, &GameOrchestrator::betMaximum);

    // Force the initial bet to be 1 credit so the UI will update
    _gameOrc->setCreditsToBet(1);

    // Return button to go back to game selection
    connect(ui->returnButton, &QPushButton::clicked, this, &GameOrchestratorWindow::close);

    // The all-important deal/draw button
    connect(ui->drawDealButton, &QPushButton::clicked, _gameOrc, &GameOrchestrator::dealDraw);

    // The text of the button should say 'deal' when a game is not in progress, but 'draw' when it is
    connect(_gameOrc, &GameOrchestrator::gameInProgress, this, &GameOrchestratorWindow::dealToDraw);

    // Display the cards
    HandWidget *PrimaryHand = new HandWidget;
    ui->primaryHand->layout()->addWidget(PrimaryHand);

    // Connect [primary] hand widgets to the game orchestrator to show results, winnings
    connect(_gameOrc, &GameOrchestrator::primaryHandUpdated, PrimaryHand, &HandWidget::winningTextAndAmount);

    // Connect [primary] hand card reveals to the widget as they're turned from the orchestrator
    connect(_gameOrc, &GameOrchestrator::primaryCardRevealed, PrimaryHand, &HandWidget::revealCard);

    // Connect the winnings display widget to the orchestrator
    connect(_gameOrc, &GameOrchestrator::gameWinnings, this, &GameOrchestratorWindow::currentWinnings);

    // Connect the account balance to the display
    connect(_gameOrc, &GameOrchestrator::updatedBalance, this, &GameOrchestratorWindow::currentBalance);

    // Card holding (for the primary hand) - set to disabled to start
    PrimaryHand->enableHolds(false);
    connect(_gameOrc, &GameOrchestrator::gameInProgress, PrimaryHand, &HandWidget::enableHolds);

    connect(PrimaryHand, &HandWidget::card1Hold, this, &GameOrchestratorWindow::holdCard1);
    connect(PrimaryHand, &HandWidget::card2Hold, this, &GameOrchestratorWindow::holdCard2);
    connect(PrimaryHand, &HandWidget::card3Hold, this, &GameOrchestratorWindow::holdCard3);
    connect(PrimaryHand, &HandWidget::card4Hold, this, &GameOrchestratorWindow::holdCard4);
    connect(PrimaryHand, &HandWidget::card5Hold, this, &GameOrchestratorWindow::holdCard5);

    // Tie the redraw status to the primary hand display
    connect(_gameOrc, &GameOrchestrator::cardsToRedraw, PrimaryHand, &HandWidget::showCardBacks);

    // Set the speed button to change the rendering rate of the cards
    connect(ui->speedButton, &QPushButton::clicked, _gameOrc, &GameOrchestrator::speedControlCycle);

    // Render the speed setting on the speed button
    connect(_gameOrc, &GameOrchestrator::renderSpeed, this, &GameOrchestratorWindow::updateSpeedChar);

    // The primary hand should recognize when a new game has started
    connect(_gameOrc, &GameOrchestrator::newGameStarted, PrimaryHand, &HandWidget::resetAll);

    // Offload the game processor to its own thread (per https://wiki.qt.io/QThreads_general_usage)
    // Using a separate thread seems to necessitate registering the type?
    qRegisterMetaType<PlayingCard>("PlayingCard");
    QThread* _gameEventProcessor = new QThread;
    _gameOrc->moveToThread(_gameEventProcessor);

    // Make sure closing the window will stop the orchestrator and thread resources
    connect(this, &GameOrchestratorWindow::destroyed, _gameEventProcessor, &QThread::quit);
    connect(this, &GameOrchestratorWindow::destroyed, _gameEventProcessor, &QThread::deleteLater);
    _gameEventProcessor->start();
}

GameOrchestratorWindow::~GameOrchestratorWindow()
{
    delete _gameOrc;
    delete _gameLogic;
    delete ui;
}

void GameOrchestratorWindow::showPayTableAndBet()
{
    // Cleanup existing entries
    qint32 initialRowCount = ui->payoutDisplay->rowCount();
    for (qint32 rowNum = 0; rowNum < initialRowCount; ++rowNum) {
        ui->payoutDisplay->removeRow(0);
    }

    // Get the new pay table from the orchestrator
    QVector<QPair<const QString, int>> payouts;
    _gameOrc->currentPayTable(payouts);

    // Scan through the entries and add them to the pay table (do not add the last one which is for no win!)
    for (qint32 rowCount = 0; rowCount < payouts.size() - 1; ++rowCount) {
        ui->payoutDisplay->insertRow(ui->payoutDisplay->rowCount());
        ui->payoutDisplay->setItem(rowCount, 0, new QTableWidgetItem(payouts[rowCount].first));
        ui->payoutDisplay->setItem(rowCount, 1, new QTableWidgetItem(QString::number(payouts[rowCount].second)));
    }

    // Show the bet amount requested
    ui->betAmount->display(static_cast<int>(_gameOrc->creditsToBet()));
}

void GameOrchestratorWindow::dealToDraw(bool showDraw)
{
    // When the game is in progress, ONLY let the draw button be pressed
    if (showDraw) {
        ui->drawDealButton->setText("Draw (/)");
        ui->drawDealButton->setShortcut(QKeySequence("/"));
        ui->winAmount->display(0);
        ui->returnButton->setDisabled(true);
        ui->betIncrementButton->setDisabled(true);
        ui->maxBetButton->setDisabled(true);
    } else {
        ui->drawDealButton->setText("Deal (/)");
        ui->drawDealButton->setShortcut(QKeySequence("/"));
        ui->returnButton->setDisabled(false);
        ui->betIncrementButton->setDisabled(false);
        ui->maxBetButton->setDisabled(false);
    }
}

void GameOrchestratorWindow::currentWinnings(quint32 winningsOfHands)
{
    ui->winAmount->display(static_cast<int>(winningsOfHands));
}

void GameOrchestratorWindow::currentBalance(quint32 totalCredits)
{
    ui->creditsAmount->display(static_cast<int>(totalCredits));
}

void GameOrchestratorWindow::updateSpeedChar(const QString &speedStr)
{
    ui->speedButton->setText("Speed " + speedStr + " (M)");
    ui->speedButton->setShortcut(QKeySequence("M"));
}

void GameOrchestratorWindow::holdCard1(bool cardHeld)
{
    _gameOrc->hold(0, cardHeld);
}

void GameOrchestratorWindow::holdCard2(bool cardHeld)
{
    _gameOrc->hold(1, cardHeld);
}

void GameOrchestratorWindow::holdCard3(bool cardHeld)
{
    _gameOrc->hold(2, cardHeld);
}

void GameOrchestratorWindow::holdCard4(bool cardHeld)
{
    _gameOrc->hold(3, cardHeld);
}

void GameOrchestratorWindow::holdCard5(bool cardHeld)
{
    _gameOrc->hold(4, cardHeld);
}
