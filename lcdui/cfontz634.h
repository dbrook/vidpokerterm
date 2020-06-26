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
    void setupCharacters();
    void clearDisplay();
    void cursorToTextPosition(int row, int col);
    void writeTextAtPos(int startRow, int startCol, const QString &text);
    void writeByteAtPos(int startRow, int startCol, const unsigned char lcdChar);

public slots:
    void fillSoftkeyText(int softkeyIdx, const QString &softkeyText);

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
