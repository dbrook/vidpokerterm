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

#ifndef CFONTZ634_H
#define CFONTZ634_H

#include "genericlcd.h"

/**
 * @brief The CFontz634 class implements the GenericLCD functions for a 20x4 character Crystalfontz 634 UART-based LCD
 *
 * @note  See the comments in GenericLCD for more details about what each member function redefined here does
 */
class CFontz634 : public GenericLCD
{
    Q_OBJECT
public:
    explicit CFontz634(const QString &device, QObject *parent = nullptr);

    ~CFontz634();

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Standard LCD features                                                                                         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /**
     * @brief setupCharacters centralized location to fill the up-to-8 CGRAM custom characters
     */
    void setupCharacters();

    /**
     * @brief clearDisplay clears all text/graphics on the display
     */
    void clearDisplay();

    /**
     * @brief cursorToTextPosition moves the text writing cursor to the desired row and column. Probably should not be
     *        used to position stuff on graphic LCDs?
     *
     * @param[in]  row            row to which the cursor should move
     * @param[in]  col            column to which the cursor should move
     */
    void cursorToTextPosition(int row, int col);

    /**
     * @brief writeTextAtPos writes a string buffer to the display, however it will not convert any special characters
     *        to the bizarre character map of the Crystalfontz 634, so ... best to stick to alphanumerics
     *
     * @param[in]  startRow       Row (0-indexed) to place the text
     * @param[in]  startCol       Column (0-indexed) to begin writing the text
     * @param[in]  text           The text to be written
     */
    void writeTextAtPos(int startRow, int startCol, const QString &text);

    /**
     * @brief writeByteAtPos write a single byte after initializing the display's cursor. Use this to send special
     *        characters from the CGROM/CGRAM of the display (for instance, any custom chars you made)
     *
     * @param[in]  startRow
     * @param[in]  startCol
     * @param[in]  lcdChar
     */
    void writeByteAtPos(int startRow, int startCol, const unsigned char lcdChar);

public slots:
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Standard softkey rendering function                                                                           *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void fillSoftkeys(QVector<QString> softkeys);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Primary Account and Game Selection Window Display Functions                                                   *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void setupWelcomeDisplay();
    void showCreditsInMainWin(quint32 nbPlayerCred);
    void showGameName(const QString &gameName);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * "Shutting down" display                                                                                       *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void showPreShutdownWarning();
    void showShutdownMessage();
    void fatalShutdownMessage();

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Generic Poker Game - Orchestrator Interaction Screen                                                          *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void setupGameDisplay();
    void showCreditsInGame(quint32 nbPlayerCred);
    void showWinnings(const QString &winString, quint32 winCredits);
    void showBetAmount(quint32 creditsBet);
    void showCardValue(int cardIdx, PlayingCard cardToShow);
    void showHoldIndicator(int cardIdx, bool isHeld);
    void showCardFrames(bool card1, bool card2, bool card3, bool card4, bool card5);
    void displayNoFundsWarning();
    void clearAllHolds();

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Generic Pay Table Interaction Screen                                                                          *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void setupPayTableDisplay(const QString &gameName);
    void displayTablePage(QVector<QPair<const QString, int>> table, int startIdx, int nbItems);

private:
    /*****************************************************************************
     * Serial / USB Port Reservation - parameters from Crystalfontz example code *
     *****************************************************************************/
    QString _deviceName;
    int     _baudRate;
    int     _deviceHandle;

    /**
     * @brief sendByte sinds a single byte using POSIX write(), since the display controller is basically a UART,
     *        command are sent to it as these numbers
     *
     * @param[in]  byte           Single byte to send to the LCD controller
     */
    void sendByte(unsigned char byte);

    /**
     * @brief sendNullTermBytes sends ASCII(~ish) text to the screen with a slight delay
     *
     * @param[in]  data           NULL-terminated old-world C-style string
     */
    void sendNullTermBytes(const char* data);
};

#endif // CFONTZ634_H
