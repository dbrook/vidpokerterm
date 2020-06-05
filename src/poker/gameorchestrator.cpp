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

GameOrchestrator::GameOrchestrator(PokerGame &gameAnalyzer,
                                   quint32    nbHandsToPlay,
                                   Account   &playerAcct,
                                   QObject   *parent)
    : _gameAnalyzer (gameAnalyzer),
      _nbHandsPerBet(nbHandsToPlay),
      _playerAccount(playerAcct),
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

GameOrchestrator::GameOrchestrator(PokerGame &gameAnalyzer,
                                   Hand      &fixedHandTest,
                                   Account   &playerAcct,
                                   QObject   *parent)
    : _gameAnalyzer (gameAnalyzer),
      _nbHandsPerBet(1),
      _playerAccount(playerAcct),
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
    _gameAnalyzer.setCreditsPerBet(credits);
}

qint8 GameOrchestrator::creditsToBet() const
{
    return _gameAnalyzer.getCreditsPerBet();
}

void GameOrchestrator::dealDraw()
{
    if (!_handInProg) {
        /*
         * First stage of the game, no cards dealt yet so deal all of them, make sure deck is full and hand is empty
         */
        if (!_fakeGame) {
            _gameCards[0].first.reset();
            _gameCards[0].second.reset();
        }

        // Must have enough credits to continue
        if (_playerAccount.balance() < _gameAnalyzer.getCreditsPerBet()) {
            qDebug() << "Insufficient funds to play a game";

            /***********************************************************************************************************
             * TODO: a signal to the game window should be added for this condition so a warning can show up
             **********************************************************************************************************/
            return;
        }

        // Take the bet amount from the account * the number of hands played
        _playerAccount.withdraw(_nbHandsPerBet * _gameAnalyzer.getCreditsPerBet());

        // Do not actually draw any cards if in a unit test simulation mode
        if (!_fakeGame) {
            // The initial deal only operates on the main hand
            try {
                for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                    _gameCards[0].second.addCard(_gameCards[0].first.drawCard());
                    // TODO: signal for each card added
                    // TODO: real games don't instantly display the cards but have a delay for each ... use a QTimer?
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }
        }
        _handInProg = true;
    } else {
        /*
         * Second stage of a game, hold cards selected, so draw only non-held-cards, then analyze the win
         */
        for (quint32 handIdx = 0; handIdx < _nbHandsPerBet; ++handIdx) {
            try {
                for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                    if (!_gameCards[0].second.cardHeld(cardIdx)) {
                        _gameCards[0].second.replaceCard(cardIdx, _gameCards[0].first.drawCard());
                    }
                    // TODO: signal for each card added
                    // TODO: real games don't instantly display the cards but delay for each ... use a QTimer?
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }

            // Analyze the final hand to see what the player has won (if anything), and adjust the balance
            _gameAnalyzer.analyzeHand(_gameCards[handIdx].second);
            _playerAccount.add(_gameAnalyzer.getWinnings());

            // TODO: analyze the win, emit the win string, amount, from the gameAnalyzer + the new account balance
        }
        _handInProg = false;
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
    qint8 currentBet = _gameAnalyzer.getCreditsPerBet();
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
    _gameAnalyzer.setCreditsPerBet(newBetAmount);

    // TODO: Signal to UI the new bet amount
}

void GameOrchestrator::betMaximum()
{
    _gameAnalyzer.setCreditsPerBet(5);

    // TODO: Signal to UI the new bet amount
}
