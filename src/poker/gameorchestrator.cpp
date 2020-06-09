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

#include "gameorchestrator.h"

#include <QDebug>
#include <QThread>

GameOrchestrator::GameOrchestrator(PokerGame *gameAnalyzer,
                                   quint32    nbHandsToPlay,
                                   Account   &playerAcct,
                                   quint8     renderDelay,
                                   QObject   *parent)
    : _gameAnalyzer (gameAnalyzer),
      _nbHandsPerBet(nbHandsToPlay),
      _playerAccount(playerAcct),
      _renderDelayMS(renderDelay),
      _fakeGame     (false),
      _handInProg   (false),
      QObject       (parent)
{
    // TODO: How many hand should we max out at? This is a fun thought experiment ... probably depends on screen reso.?
    _gameCards.reserve(nbHandsToPlay);
    for (quint32 handCount = 0; handCount < nbHandsToPlay; ++handCount) {
        Hand pokerHand;
        Deck cardDeck(Deck::FULL_FRENCH);
        QPair<Deck, Hand> singleDeckHand(cardDeck, pokerHand);
        _gameCards.push_back(singleDeckHand);
    }
}

GameOrchestrator::GameOrchestrator(PokerGame *gameAnalyzer,
                                   Hand      &fixedHandTest,
                                   Account   &playerAcct,
                                   QObject   *parent)
    : _gameAnalyzer (gameAnalyzer),
      _nbHandsPerBet(1),
      _playerAccount(playerAcct),
      _renderDelayMS(0),
      _fakeGame     (true),
      _handInProg   (false),
      QObject       (parent)
{
    _gameCards.reserve(1);
    Deck cardDeckToIgnore(Deck::FULL_FRENCH);
    QPair<Deck, Hand> singleDeckHand(cardDeckToIgnore, fixedHandTest);
    _gameCards.push_back(singleDeckHand);

    for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
        _gameCards[0].second.holdCard(cardIdx, true);
    }
}

Hand GameOrchestrator::retrieveHand(qint32 handNumber) const
{
    if (handNumber >= _gameCards.size()) {
        throw std::runtime_error("Requested hand was out of range");
    }
    return _gameCards[handNumber].second;
}

void GameOrchestrator::setCreditsToBet(qint8 credits)
{
    _gameAnalyzer->setCreditsPerBet(credits);
    emit betUpdated();
}

qint8 GameOrchestrator::creditsToBet() const
{
    return _gameAnalyzer->getCreditsPerBet();
}

void GameOrchestrator::currentPayTable(QVector<QPair<const QString, int> > &payTable)
{
    _gameAnalyzer->currentPayTable(payTable);
}

bool GameOrchestrator::isGameInProgress() const
{
    return _handInProg;
}

void GameOrchestrator::dealDraw()
{
    if (!_handInProg) {
        /*
         * First stage of the game, no cards dealt so ensure deck is full + shuffled and the target hand(s) empty
         */
        emit newGameStarted();

        if (!_fakeGame) {
            _gameCards[0].first.reset();
            _gameCards[0].second.reset();
            _gameCards[0].first.shuffle();
        }

        // Must have enough credits to continue
        if (_playerAccount.balance() < _gameAnalyzer->getCreditsPerBet()) {
            qDebug() << "Insufficient funds to play a game";

            /***********************************************************************************************************
             * TODO: a signal to the game window should be added for this condition so a warning can show up
             **********************************************************************************************************/
            return;
        }

        // Set the in progress state right away so the UI will be updated before dealing out cards
        _handInProg = true;
        emit gameInProgress(_handInProg);

        // Take the bet amount from the account * the number of hands played
        _playerAccount.withdraw(_nbHandsPerBet * _gameAnalyzer->getCreditsPerBet());
        emit updatedBalance(_playerAccount.balance());

        // Do not actually draw any cards if in a unit test simulation mode
        if (!_fakeGame) {
            // The initial deal only operates on the main hand
            try {
                for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                    // Actual terminals like to give the appearance of a game, so introduce a delay between showing
                    if (_renderDelayMS != 0)
                        QThread::msleep(_renderDelayMS);

                    PlayingCard nextCard = _gameCards[0].first.drawCard();
                    _gameCards[0].second.addCard(nextCard);
                    emit primaryCardRevealed(cardIdx, nextCard);
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }

            // The nice poker terminals tell you what you have at the first deal (even though you haven't "won" yet)
            // So it is ok to analyze the hand at the deal, so long as we don't "count" the winnings
            _gameAnalyzer->analyzeHand(_gameCards[0].second);
            emit primaryHandUpdated(_gameAnalyzer->handResult(), 0);
        }
    } else {
        /*
         * Second stage of a game, hold cards selected, so draw only non-held-cards, then analyze the win
         */
        // First, flip the cards back over
        bool flipCard1 = false;
        bool flipCard2 = false;
        bool flipCard3 = false;
        bool flipCard4 = false;
        bool flipCard5 = false;
        if (!_gameCards[0].second.cardHeld(0)) {
            flipCard1 = true;
        }
        if (!_gameCards[0].second.cardHeld(1)) {
            flipCard2 = true;
        }
        if (!_gameCards[0].second.cardHeld(2)) {
            flipCard3 = true;
        }
        if (!_gameCards[0].second.cardHeld(3)) {
            flipCard4 = true;
        }
        if (!_gameCards[0].second.cardHeld(4)) {
            flipCard5 = true;
        }
        emit cardsToRedraw(flipCard1, flipCard2, flipCard3, flipCard4, flipCard5);

        // ... then reveal them
        quint32 totalWinnings = 0;
        for (quint32 handIdx = 0; handIdx < _nbHandsPerBet; ++handIdx) {
            try {
                for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                    if (!_gameCards[0].second.cardHeld(cardIdx)) {
                        // Actual terminals like to give the appearance of a game, so introduce a delay between showing
                        if (_renderDelayMS != 0)
                            QThread::msleep(_renderDelayMS);

                        PlayingCard nextCard = _gameCards[0].first.drawCard();
                        _gameCards[0].second.replaceCard(cardIdx, nextCard);
                        emit primaryCardRevealed(cardIdx, nextCard);
                    }
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }

            // Analyze the final hand to see what the player has won (if anything), and adjust the balance
            _gameAnalyzer->analyzeHand(_gameCards[handIdx].second);
            _playerAccount.add(_gameAnalyzer->getWinnings());

            // TODO: analyze the win, emit the win string, amount, from the gameAnalyzer + the new account balance
            // TODO: so far just the primary hand...
            emit primaryHandUpdated(_gameAnalyzer->handResult(), _gameAnalyzer->getWinnings());
            totalWinnings += _gameAnalyzer->getWinnings();
            emit gameWinnings(totalWinnings);
        }
        _handInProg = false;
        emit gameInProgress(_handInProg);
        emit updatedBalance(_playerAccount.balance());
    }
}

void GameOrchestrator::hold(quint8 cardPosition, bool canHold)
{
    // Do nothing if a hold was requested for a non-existent card or no hand is currently in progress
    if (!_handInProg || cardPosition >= Hand::kCardsPerHand) {
        return;
    }

    // Otherwise set the hold
    try {
        _gameCards[0].second.holdCard(cardPosition, canHold);
    } catch (std::runtime_error &exception) {
        qDebug() << "WARNING: " << exception.what();
    }

    // TODO: must repeat for each hand being played and emit each card across all hands
}

void GameOrchestrator::cycleBetAmount()
{
    qint8 currentBet = _gameAnalyzer->getCreditsPerBet();
    qint8 newBetAmount;
    switch (currentBet) {
    case 1:
        newBetAmount = 2;
        break;
    case 2:
        newBetAmount = 3;
        break;
    case 3:
        newBetAmount = 4;
        break;
    case 4:
        newBetAmount = 5;
        break;
    case 5:
    default:
        newBetAmount = 1;
    }
    _gameAnalyzer->setCreditsPerBet(newBetAmount);
    emit betUpdated();
}

void GameOrchestrator::betMaximum()
{
    _gameAnalyzer->setCreditsPerBet(5);
    emit betUpdated();
}

void GameOrchestrator::speedControlCycle()
{
    if (_renderDelayMS == 150) {
        _renderDelayMS = 100;
        emit renderSpeed(">>");
    } else if (_renderDelayMS == 100) {
        _renderDelayMS = 50;
        emit renderSpeed(">>>");
    } else if (_renderDelayMS == 50) {
        _renderDelayMS = 0;
        emit renderSpeed(">>>>");
    } else {
        _renderDelayMS = 150;
        emit renderSpeed(">");
    }
}
