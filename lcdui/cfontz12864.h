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

#ifndef CFONTZ12864_H
#define CFONTZ12864_H

#include "genericlcd.h"

#include "u8g2.h"

/**
 * @brief The Cfontz12864 class handles SPI communication using WiringPi and u8g2
 *
 * @note  See the comments in GenericLCD for more details about what each member function redefined here does
 *
 * @warning in order to use the display, the wiringPiSetup() function must be run prior to instantiating an object
 */
class CFontz12864 : public GenericLCD
{
    Q_OBJECT
public:
    CFontz12864(QObject *parent = nullptr);

    ~CFontz12864();

    // TODO: as code is written, see if any refactoring can be done. It's harder with this display because it is far
    //       more advanced than the character-only ones

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

private:
    u8g2_t _disp;

    unsigned char _heart_bitmap[18];
    unsigned char _diamond_bitmap[18];
    unsigned char _spade_bitmap[18];
    unsigned char _club_bitmap[18];
};

#endif // CFONTZ12864_H
