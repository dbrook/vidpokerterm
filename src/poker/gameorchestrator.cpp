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
    : QObject       (parent),
      _gameAnalyzer (gameAnalyzer),
      _nbHandsToPlay(nbHandsToPlay),
      _betsPerHand  (1),
      _playerAccount(playerAcct),
      _renderDelayMS(renderDelay),
      _fakeGame     (false),
      _handInProg   (false)
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
    : QObject       (parent),
      _gameAnalyzer (gameAnalyzer),
      _nbHandsToPlay(1),
      _betsPerHand  (1),
      _playerAccount(playerAcct),
      _renderDelayMS(0),
      _fakeGame     (true),
      _handInProg   (false)
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

void GameOrchestrator::setCreditsToBet(qint32 credits)
{
    _betsPerHand = credits;
    emit betUpdated();
}

qint8 GameOrchestrator::creditsToBet() const
{
    return _betsPerHand;
}

void GameOrchestrator::currentPayTable(QVector<QPair<const QString, int> > &payTable)
{
    _gameAnalyzer->currentPayTable(_betsPerHand, payTable);
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
         * The player only interacts with the first hand, so the others will only be initialized and shuffled
         */
        if (!_fakeGame) {
            for (QPair<Deck, Hand> &handDeck : _gameCards) {
                handDeck.first.reset();
                handDeck.second.reset();
            }
            // Only shuffle the deck the player is interacting with. The secondary decks will be shuffled at draw time.
            _gameCards[0].first.shuffle();
        }

        // Must have enough credits to continue
        if (_playerAccount.balance() < _betsPerHand * _nbHandsToPlay) {
            qDebug() << "Insufficient funds to play a game";

            /***********************************************************************************************************
             * TODO: a signal to the game window should be added for this condition so a warning can show up
             **********************************************************************************************************/
            return;
        }

        // Set the in progress state right away so the UI will be updated before dealing out cards
        _handInProg = true;
        emit gameInProgress(_handInProg);
        emit operating(true);

        // Take the bet amount from the account * the number of hands played
        _playerAccount.withdraw(_nbHandsToPlay * _betsPerHand);

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

                // For all secondary hands, fill them with null / placeholder cards
                for (int handIdx = 1; handIdx < _gameCards.size(); ++handIdx) {
                    for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                        _gameCards[handIdx].second.addCard(PlayingCard());
                    }
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }

            // The nice poker terminals tell you what you have at the first deal (even though you haven't "won" yet)
            // So it is ok to analyze the hand at the deal, so long as we don't "count" the winnings
            QString handAnalyResult;
            quint32 handWinCredits;
            _gameAnalyzer->determineHandAndWin(_gameCards[0].second, _betsPerHand, handAnalyResult, handWinCredits);
            emit primaryHandUpdated(handAnalyResult, 0);
            emit operating(false);
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
        emit operating(true);

        // ... then reveal them
        quint32 totalWinnings = 0;
        for (quint32 handIdx = 0; handIdx < _nbHandsToPlay; ++handIdx) {
            // Shuffle all secondary decks before drawing!
            if (handIdx != 0) {
                _gameCards[handIdx].first.shuffle();
            }

            try {
                for (quint32 cardIdx = 0; cardIdx < Hand::kCardsPerHand; ++cardIdx) {
                    if (!_gameCards[handIdx].second.cardHeld(cardIdx)) {
                        // Actual terminals like to give the appearance of a game, so introduce a delay between showing
                        if (_renderDelayMS != 0)
                            QThread::msleep(_renderDelayMS);

                        PlayingCard nextCard = _gameCards[handIdx].first.drawCard();
                        _gameCards[handIdx].second.replaceCard(cardIdx, nextCard);

                        if (handIdx == 0) {
                            // Primary hand cards
                            emit primaryCardRevealed(cardIdx, nextCard);
                        } else {
                            // Secondary hand(s) cards
                            emit secondaryCardRevealed(handIdx - 1, cardIdx, nextCard, true);
                        }
                    }
                }
            } catch (std::runtime_error &exception) {
                qDebug() << "WARNING: " << exception.what();
                return;
            }

            // Analyze the final hand to see what the player has won (if anything), and adjust the balance
            QString handAnalyRslt;
            quint32 handWinCreds;
            _gameAnalyzer->determineHandAndWin(_gameCards[handIdx].second, _betsPerHand, handAnalyRslt, handWinCreds);
            _playerAccount.add(handWinCreds);

            // Update the front-end with the individual hand winnings and total winnings (for multi-handed games)
            if (handIdx == 0) {
                // Primary Hand Analysis
                emit primaryHandUpdated(handAnalyRslt, handWinCreds);
            } else {
                // Secondary Hand Analysis
                emit secondaryHandUpdated(handIdx - 1, handAnalyRslt, handWinCreds);
            }
            totalWinnings += handWinCreds;
            emit gameWinnings(totalWinnings);
        }
        _handInProg = false;
        emit gameInProgress(_handInProg);
        emit operating(false);
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
        // First the primary hand
        _gameCards[0].second.holdCard(cardPosition, canHold);

        // See which card was actually held so it can be removed from any secondary decks
        const PlayingCard heldCard = _gameCards[0].second.cardAt(cardPosition);

        // Then the secondary hands (+ emit to the orchestrator UI that there is a card to show or hide)
        for (int secondaryIdx = 1; secondaryIdx < _gameCards.size(); ++secondaryIdx) {
            if (canHold) {
                // Holding a card will take it out of the deck
                _gameCards[secondaryIdx].first.removeCard(heldCard);
                _gameCards[secondaryIdx].second.replaceCard(cardPosition, heldCard);
                _gameCards[secondaryIdx].second.holdCard(cardPosition, true);
                emit secondaryCardRevealed(secondaryIdx - 1, cardPosition, heldCard, true);
            } else {
                // Un-Holding a card will put it back
                // WARNING: as a result of this, the deck will lose its randomness, so secondary decks must be shuffled
                //          before drawing from for the final hand(s) conditions.
                _gameCards[secondaryIdx].second.replaceCard(cardPosition, PlayingCard());
                _gameCards[secondaryIdx].first.addCard(heldCard);
                _gameCards[secondaryIdx].second.holdCard(cardPosition, false);
                emit secondaryCardRevealed(secondaryIdx - 1, cardPosition, heldCard, false);
            }
        }
    } catch (std::runtime_error &exception) {
        qDebug() << "WARNING: " << exception.what();
    }
}

void GameOrchestrator::cycleBetAmount()
{
    qint8 currentBet = _betsPerHand;
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
    _betsPerHand = newBetAmount;
    emit betUpdated();
}

void GameOrchestrator::betMaximum()
{
    _betsPerHand = 5;
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
