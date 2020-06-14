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

GameOrchestratorWindow::GameOrchestratorWindow(Account   &playerAccount,
                                               PokerGame *gameLogic,
                                               int        handsToPlay,
                                               QWidget   *parent)
    : QMainWindow(parent),
      _playerCredits(playerAccount),
      _gameLogic(gameLogic),
      _handsToPlay(handsToPlay),
      ui(new Ui::GameOrchestratorWindow)
{
    // Setup the UI
    ui->setupUi(this);

    // Now bring in the game logic
    _gameOrc = new GameOrchestrator(_gameLogic, _handsToPlay, _playerCredits, 0);

    // Fill in the number of credits the first time
    ui->creditsAmount->setText(QString::number(_playerCredits.balance()));

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

    /*
     * Dealing / Drawing cards from the orchestrator
     */
    // The all-important deal/draw button
    // -- calls our own slot which will call the Orchestrator, thread sync issues observed on slow processors otherwise
    connect(ui->drawDealButton, &QPushButton::clicked, this, &GameOrchestratorWindow::syncDealDraw);
    connect(this, &GameOrchestratorWindow::readyForDealDraw, _gameOrc, &GameOrchestrator::dealDraw);

    // While the orchestrator is working, it should not be burdened with deal-draw requests
    connect(_gameOrc, &GameOrchestrator::operating, this, &GameOrchestratorWindow::disableDealDraw);

    // The text of the button should say 'deal' when a game is not in progress, but 'draw' when it is
    connect(_gameOrc, &GameOrchestrator::gameInProgress, this, &GameOrchestratorWindow::dealToDraw);

    // Connect the winnings display widget to the orchestrator
    connect(_gameOrc, &GameOrchestrator::gameWinnings, this, &GameOrchestratorWindow::currentWinnings);

    // Connect the account balance to the display
    connect(&_playerCredits, &Account::balanceChanged, this, &GameOrchestratorWindow::currentBalance);

    /*
     * Primary hand setup: this hand will control the hold status of *all* additional hands for multi-hand games
     */
    HandWidget *PrimaryHand = new HandWidget(false, QSize(125, 175), "32", "", this);
    ui->primaryHand->layout()->addWidget(PrimaryHand);

    // Connect [primary] hand widgets to the game orchestrator to show results, winnings
    connect(_gameOrc, &GameOrchestrator::primaryHandUpdated, PrimaryHand, &HandWidget::winningTextAndAmount);

    // Connect [primary] hand card reveals to the widget as they're turned from the orchestrator
    connect(_gameOrc, &GameOrchestrator::primaryCardRevealed, PrimaryHand, &HandWidget::revealCard);

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

    // All of the hands should recognize when a new game has started
    connect(this, &GameOrchestratorWindow::resetCardDisplay, PrimaryHand, &HandWidget::resetAll);
    connect(this, &GameOrchestratorWindow::resetCardDisplay, this, &GameOrchestratorWindow::flipAllHands);

    /*
     * Additional Hands (optional)
     * Using push_front so that when rendering the hands it will go from bottom to top
     */
    ui->multipleHand->layout()->deleteLater();
    QGridLayout *secondaryHandsLayout = new QGridLayout(this);
    ui->multipleHand->setLayout(secondaryHandsLayout);
    _addedHands.reserve(_handsToPlay);

    if (_handsToPlay == 3) {
        for (int extraHands = 1; extraHands < _handsToPlay; ++extraHands) {
            HandWidget *ExtraHand = new HandWidget(true, QSize(125, 175), "32", "", this);
            secondaryHandsLayout->addWidget(ExtraHand, extraHands - 1, 0);
            _addedHands.push_front(ExtraHand);
        }
    } else if (_handsToPlay == 5) {
        int column = 1;
        int row    = 0;
        for (int extraHands = 1; extraHands < _handsToPlay; ++extraHands) {
            HandWidget *ExtraHand = new HandWidget(true, QSize(80, 110), "24", "", this);
            secondaryHandsLayout->addWidget(ExtraHand, row, column);
            _addedHands.push_front(ExtraHand);

            // Determine the row and column of the next hand to render
            column = column - 1;
            if (column == -1) {
                ++row;
                column = 1;
            }
        }
    } else if (_handsToPlay == 10) {
        int column = 2;
        int row    = 0;
        for (int extraHands = 1; extraHands < _handsToPlay; ++extraHands) {
            HandWidget *ExtraHand = new HandWidget(true, QSize(60, 80), "18", "", this);
            secondaryHandsLayout->addWidget(ExtraHand, row, column);
            _addedHands.push_front(ExtraHand);

            // Determine the row and column of the next hand to render
            column = column - 1;
            if (column == -1) {
                ++row;
                column = 2;
            }
        }
    } else if (_handsToPlay == 25) {
        int column = 4;
        int row    = 0;
        for (int extraHands = 1; extraHands < _handsToPlay; ++extraHands) {
            // Need to shrink down the payout text when there are this many shown
            HandWidget *ExtraHand = new HandWidget(true, QSize(25, 35), "9", "9", this);
            secondaryHandsLayout->addWidget(ExtraHand, row, column);
            _addedHands.push_front(ExtraHand);

            // Determine the row and column of the next hand to render
            column = column - 1;
            if (column == -1) {
                ++row;
                column = 4;
            }
        }
    } else if (_handsToPlay == 100) {
        // Mostly a thought experiment ... this runs really poorly on the Pi Zero and the HandWidget probably needs
        // some reengineering to support hands of this size on 1080p screens
        int column = 9;
        int row    = 0;
        for (int extraHands = 1; extraHands < _handsToPlay; ++extraHands) {
            HandWidget *ExtraHand = new HandWidget(true, QSize(35, 50), "9", "", this);
            secondaryHandsLayout->addWidget(ExtraHand, row, column);
            _addedHands.push_front(ExtraHand);

            // Determine the row and column of the next hand to render
            column = column - 1;
            if (column == -1) {
                ++row;
                column = 9;
            }
        }
    }

    connect(_gameOrc, &GameOrchestrator::secondaryCardRevealed, this, &GameOrchestratorWindow::updateSecondaryHandCard);
    connect(_gameOrc, &GameOrchestrator::secondaryHandUpdated, this, &GameOrchestratorWindow::secondaryWinTextAndAmt);

    /*
     * Render Speed Control
     */
    // Set the speed button to change the rendering rate of the cards
    connect(ui->speedButton, &QPushButton::clicked, _gameOrc, &GameOrchestrator::speedControlCycle);

    // Render the speed setting on the speed button
    connect(_gameOrc, &GameOrchestrator::renderSpeed, this, &GameOrchestratorWindow::updateSpeedChar);

    /*
     * Offload the game processor to its own thread (per https://wiki.qt.io/QThreads_general_usage)
     * Using a separate thread seems to necessitate registering the type?
     */
    qRegisterMetaType<PlayingCard>("PlayingCard");
    QThread* _gameEventProcessor = new QThread;
    _gameOrc->moveToThread(_gameEventProcessor);

    // Make sure closing the window will stop the orchestrator and thread resources
    connect(this, &GameOrchestratorWindow::destroyed, _gameEventProcessor, &QThread::quit);
    connect(this, &GameOrchestratorWindow::destroyed, _gameEventProcessor, &QThread::deleteLater);

    _gameEventProcessor->start();

    // Uncomment to make the game full-screen
//    this->setWindowState(this->windowState() | Qt::WindowFullScreen);
}

GameOrchestratorWindow::~GameOrchestratorWindow()
{
    // Cleanup heap-allocated objects
    delete _gameOrc;
    delete ui;
}

void GameOrchestratorWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    fixPayoutTableSize();
}

void GameOrchestratorWindow::fixPayoutTableSize()
{
    int totalTableWidth = ui->payoutDisplay->width();
    ui->payoutDisplay->setColumnWidth(0, totalTableWidth * 0.7);
    ui->payoutDisplay->setColumnWidth(1, totalTableWidth * 0.3);
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
        ui->payoutDisplay->item(rowCount, 1)->setTextAlignment(Qt::AlignRight);
    }

    fixPayoutTableSize();

    // Show the bet amount requested
    ui->betAmount->setText(QString::number(_gameOrc->creditsToBet() * _handsToPlay));
}

void GameOrchestratorWindow::dealToDraw(bool showDraw)
{
    // When the game is in progress, ONLY let the draw button be pressed
    if (showDraw) {
        ui->drawDealButton->setText("Draw");
        ui->drawDealButton->setDisabled(false);
        ui->drawDealButton->setShortcut(QKeySequence("/"));
        ui->winAmount->setText("0");
        ui->returnButton->setDisabled(true);
        ui->betIncrementButton->setDisabled(true);
        ui->maxBetButton->setDisabled(true);
    } else {
        ui->drawDealButton->setText("Deal");
        ui->drawDealButton->setDisabled(false);
        ui->drawDealButton->setShortcut(QKeySequence("/"));
        ui->returnButton->setDisabled(false);
        ui->returnButton->setShortcut(QKeySequence("N"));
        ui->betIncrementButton->setDisabled(false);
        ui->maxBetButton->setDisabled(false);
    }
}

void GameOrchestratorWindow::disableDealDraw(bool dealDrawDisabled)
{
    // Do not let the deal/draw button get pressed again while the orchestrator is working
    ui->drawDealButton->setDisabled(dealDrawDisabled);

    // Do not let the speed adjuster get pressed either
    ui->speedButton->setDisabled(dealDrawDisabled);
}

void GameOrchestratorWindow::syncDealDraw()
{
    if (!_gameOrc->isGameInProgress()) {
        emit resetCardDisplay();
    }

    emit readyForDealDraw();
}

void GameOrchestratorWindow::currentWinnings(quint32 winningsOfHands)
{
    ui->winAmount->setText(QString::number(winningsOfHands));
}

void GameOrchestratorWindow::currentBalance(quint32 totalCredits)
{
    ui->creditsAmount->setText(QString::number(totalCredits));
}

void GameOrchestratorWindow::updateSpeedChar(const QString &speedStr)
{
    ui->speedButton->setText("Speed " + speedStr);
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

void GameOrchestratorWindow::updateSecondaryHandCard(int secoHandPos, int cardPos, PlayingCard cardToShow, bool show)
{
    if (show) {
        _addedHands[secoHandPos]->revealCard(cardPos, cardToShow);
    } else {
        bool flipcard1 = false;
        bool flipcard2 = false;
        bool flipcard3 = false;
        bool flipcard4 = false;
        bool flipcard5 = false;
        switch (cardPos) {
        case 0:
            flipcard1 = true;
            break;
        case 1:
            flipcard2 = true;
            break;
        case 2:
            flipcard3 = true;
            break;
        case 3:
            flipcard4 = true;
            break;
        case 4:
            flipcard5 = true;
            break;
        };

        _addedHands[secoHandPos]->showCardBacks(flipcard1, flipcard2, flipcard3, flipcard4, flipcard5);
    }
}

void GameOrchestratorWindow::secondaryWinTextAndAmt(int secoHandPos, const QString &handString, quint32 winning)
{
    _addedHands[secoHandPos]->winningTextAndAmount(handString, winning);
}

void GameOrchestratorWindow::flipAllHands()
{
    for (HandWidget *secoHand : _addedHands) {
        secoHand->resetAll();
    }
}
