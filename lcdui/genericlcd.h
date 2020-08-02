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

#ifndef GENERICLCD_H
#define GENERICLCD_H

#include "playingcard.h"

#include <QObject>
#include <QVector>

/**
 * @brief The GenericLCD class allows for a standardized interface for a Poker Game Orchestrator, Pay Table, and Game
 *        interface to display information to any sub-classed LCD type. The general idea is to:
 *
 *        +-------------------+
 *        |                   | ---+
 *        |                   |    +-- program area
 *        |                   | ---+
 *        |     |     |     |>| <----- softkey area (3 features can be displayed + a "next page" button)
 *        +-------------------+
 */
class GenericLCD : public QObject
{
    Q_OBJECT
public:
    explicit GenericLCD(QObject *parent = nullptr);

    virtual ~GenericLCD();

public slots:
    /**
     * @brief fillSoftkeys renders the text of the specified softkey to the softkey region
     *
     * @param[in]  softkeys       array of softkeys to show on the screen
     */
    virtual void fillSoftkeys(QVector <QString> softkeys) = 0;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Primary Account and Game Selection Window Display Functions                                                   *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /**
     * @brief setupWelcomeDisplay renders an application title and puts placeholders for the dynamically-set widgets
     */
    virtual void setupWelcomeDisplay() = 0;

    /**
     * @brief showCreditsInMainWin displays the number of credits available to the "Credits" field of the display
     *
     * @param[in]  nbPlayerCred   Number of credits the player has available
     */
    virtual void showCreditsInMainWin(quint32 nbPlayerCred) = 0;

    /**
     * @brief showGameName renders the name of the currently selected game to the "Game" field of the display
     *
     * @param[in]  gameName       Name of the game that will be run when the player selects the "PLAY" softkey
     */
    virtual void showGameName(const QString &gameName) = 0;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * "Shutting down" displays                                                                                      *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    virtual void showPreShutdownWarning() = 0;
    virtual void showShutdownMessage() = 0;
    virtual void fatalShutdownMessage() = 0;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Generic Poker Game - Orchestrator Interaction Screen                                                          *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /**
     * @brief setupGameDisplay takes over the whole display to view the state of a game orchestrator
     */
    virtual void setupGameDisplay() = 0;

    /**
     * @brief showCreditsInGame displays the number of credits a player has in the game display
     *
     * @param[in]  nbPlayerCred   number of credits available
     */
    virtual void showCreditsInGame(quint32 nbPlayerCred) = 0;

    /**
     * @brief showWinnings displays the winning credits and the string representing the hand of the win
     *
     * @param[in]  winString      name of the hand that was matched for the win
     * @param[in]  winCredits     number of credits won
     */
    virtual void showWinnings(const QString &winString, quint32 winCredits) = 0;

    /**
     * @brief showBetAmount displays the number of credits a player has "invested" into the hand
     *
     * @param[in]  creditsBet     number of credits each hand costs to bet
     */
    virtual void showBetAmount(quint32 creditsBet) = 0;

    /**
     * @brief showCardValue "flips" a card so its suit and rank show
     *
     * @param[in]  cardIdx        position of the card in the hand to show
     * @param[in]  cardToShow     actual card to show in the hand position
     */
    virtual void showCardValue(int cardIdx, PlayingCard cardToShow) = 0;

    /**
     * @brief showHoldIndicator highlights something around the card to show it will be preserved when redrawing
     *
     * @param[in]  cardIdx        position of the card in the hand to mark as held
     */
    virtual void showHoldIndicator(int cardIdx, bool isHeld) = 0;

    /**
     * @brief showCardFrames shows the "backs" of requested cards
     */
    virtual void showCardFrames(bool card1, bool card2, bool card3, bool card4, bool card5) = 0;

    /**
     * @brief displayNoFundsWarning will overlay a warning there are insufficient funds to play
     */
    virtual void displayNoFundsWarning() = 0;

    /**
     * @brief clearAllHolds erases all the hold indicators in one write
     */
    virtual void clearAllHolds() = 0;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Generic Pay Table Interaction Screen                                                                          *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /**
     * @brief setupPayTableDisplay clears the screen and sets the title bar to the game name
     */
    virtual void setupPayTableDisplay(const QString &gameName) = 0;

    /**
     * @brief displayTablePage renders [part of] the paytable passed in 'table'
     */
    virtual void displayTablePage(QVector<QPair<const QString, int>> table, int startIdx, int nbItems) = 0;

signals:
    void shutdownDisplayed();
};

#endif // GENERICLCD_H
