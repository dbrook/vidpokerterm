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

/**
 * @brief The GenericLCD class allows for a standardized interface for a Poker Game Orchestrator, Pay Table, and Game
 *        interface to display information to any sub-classed LCD type. The general idea is to:
 *
 *        +-------------------+
 *        |                   | ---+
 *        |                   |    +-- program area
 *        |                   | ---+
 *        |   |   |   |   |   | <----- softkey area (4 features can be displayed like in the desktop interface)
 *        +-------------------+
 */
class GenericLCD : public QObject
{
    Q_OBJECT
public:
    explicit GenericLCD(QObject *parent = nullptr);

    virtual ~GenericLCD();

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Standard LCD features                                                                                         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /**
     * @brief clearDisplay clears all text/graphics on the display
     */
    virtual void clearDisplay() = 0;

    /**
     * @brief cursorToTextPosition moves the text writing cursor to the desired row and column. Probably should not be
     *        used to position stuff on graphic LCDs?
     *
     * @param[in]  row            row to which the cursor should move
     * @param[in]  col            column to which the cursor should move
     */
    virtual void cursorToTextPosition(int row, int col) = 0;

    /**
     * @brief      writeTextAtPos will display a string on the LCD starting at startRow, startCol
     *
     * @param[in]  startRow       Row at which to start writing
     * @param[in]  startCol       Column at which to start writing
     * @param[in]  text           The text to be written
     */
    virtual void writeTextAtPos(int startRow, int startCol, const QString &text) = 0;

    /**
     * @brief fillSoftkeyText renders the text of the specified softkey to the softkey region
     *
     * @param[in]  softkeyIdx     0, 1, 2, 3 for the softkey position
     * @param[in]  softkeyText    the text to display -- should be truncated if too long for the text region!
     */
    virtual void fillSoftkeyText(int softkeyIdx, const QString &softkeyText) = 0;

public slots:
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
};

#endif // GENERICLCD_H
