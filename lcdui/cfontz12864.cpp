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

#include "cfontz12864.h"

#include "u8g2_hal_rpi.h"

#include <wiringPi.h>

CFontz12864::CFontz12864(QObject *parent)
    : GenericLCD(parent),
      _heart_bitmap
        {0x6c, 0x00, 0x92, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x82, 0x00, 0x44, 0x00, 0x28, 0x00, 0x10, 0x00},
      _diamond_bitmap
        {0x10, 0x00, 0x28, 0x00, 0x44, 0x00, 0x82, 0x00, 0x01, 0x01, 0x82, 0x00, 0x44, 0x00, 0x28, 0x00, 0x10, 0x00},
      _spade_bitmap
        {0x10, 0x00, 0x38, 0x00, 0x7c, 0x00, 0xfe, 0x00, 0xff, 0x01, 0xff, 0x01, 0xfe, 0x00, 0x10, 0x00, 0x7c, 0x00},
      _club_bitmap
        {0x10, 0x00, 0x38, 0x00, 0x38, 0x00, 0xd6, 0x00, 0xff, 0x01, 0xd6, 0x00, 0x10, 0x00, 0x10, 0x00, 0x7c, 0x00}
{
    // Raspberry Pi Hardware Abstraction Layer to properly setup the pins + half-duplex communication
    u8g2_rpi_hal_t u8g2_rpi_hal = {14 /* CLOCK / EN */,
                                   12 /* MOSI / RW  */,
                                   10 /* CS / RS    */};
    u8g2_rpi_hal_init(u8g2_rpi_hal);

    // Per Rafael Ibasco (https://github.com/ribasco/u8g2-rpi-demo/blob/master/main.cpp)
    // However, we don't use the U8G2_R2, but instead U8G2_R0 --> this avoids flipping the display upside-down!
    // The CFAG12864S has a 6-o'clock viewing angle so being flipped makes it painfully difficult to read.
    u8g2_Setup_st7920_s_128x64_f(&_disp, U8G2_R0, cb_byte_spi_hw, cb_gpio_delay_rpi);

    // Display must be initialized for use -- from this point on, it should be safe to use all u8g2_* functions
    u8g2_InitDisplay(&_disp);
    u8g2_SetPowerSave(&_disp, 0);
    u8g2_ClearDisplay(&_disp);
}

CFontz12864::~CFontz12864()
{
    // It seems that due to the timing of shutting down, this is not always reliably done?
    u8g2_ClearDisplay(&_disp);
}

void CFontz12864::fillSoftkeys(QVector<QString> softkeys)
{
    // Clear the box first
    u8g2_SetDrawColor(&_disp, 0);
    u8g2_DrawBox(&_disp,   0, 52, 128, 12);

    // Draw the keys if there is actual text
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_SetDrawColor(&_disp, 1);

    if (!softkeys[0].isEmpty()) {
        u8g2_DrawHLine(&_disp,   0, 52, 37);
        u8g2_DrawVLine(&_disp,  37, 52, 12);
        u8g2_DrawStr(&_disp,     0, 62, softkeys[0].toUtf8());
    }
    if (!softkeys[1].isEmpty()) {
        u8g2_DrawHLine(&_disp,  38, 52, 39);
        u8g2_DrawVLine(&_disp,  77, 52, 12);
        u8g2_DrawStr(&_disp,    40, 62, softkeys[1].toUtf8());
    }
    if (!softkeys[2].isEmpty()) {
        u8g2_DrawHLine(&_disp,  78, 52, 39);
        u8g2_DrawVLine(&_disp, 117, 52, 12);
        u8g2_DrawStr(&_disp,    80, 62, softkeys[2].toUtf8());
    }
    if (!softkeys[3].isEmpty()) {
        u8g2_DrawHLine(&_disp, 117, 52, 10);
        u8g2_DrawVLine(&_disp, 117, 52, 12);
        u8g2_DrawStr(&_disp,   120, 62, softkeys[3].toUtf8());
    }

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::setupWelcomeDisplay()
{
    // Empty out the display buffer
    u8g2_ClearBuffer(&_disp);

    // Draw some text
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);

    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_DrawStr(&_disp, 28, 8, "VidPokerTerm");

    // Draw frames for credits and game selection
    u8g2_DrawFrame(&_disp, 2, 11, 124, 14);
    u8g2_DrawFrame(&_disp, 2, 26, 124, 25);
    u8g2_DrawStr(&_disp, 4, 21, "Credits: ");
    u8g2_DrawStr(&_disp, 4, 36, "Game Selection: ");

    // Be done with it ... let's just see if this works, mmmkay?
    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showCreditsInMainWin(quint32 nbPlayerCred)
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);

    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_DrawStr(&_disp, 58, 21, QString::number(nbPlayerCred).rightJustified(11, ' ').toUtf8());

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showGameName(const QString &gameName)
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);

    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_DrawStr(&_disp, 4, 47, gameName.leftJustified(20, ' ').toUtf8());

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showPreShutdownWarning()
{
    u8g2_ClearBuffer(&_disp);
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);

    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_DrawStr(&_disp, 0,  8, "Press any button to");
    u8g2_DrawStr(&_disp, 0, 19, "continue shutdown.");

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showShutdownMessage()
{
    u8g2_ClearBuffer(&_disp);
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);

    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);
    u8g2_DrawStr(&_disp, 0, 29, "Shutting down ...");
    u8g2_DrawStr(&_disp, 0, 40, "Wait for green light");
    u8g2_DrawStr(&_disp, 0, 51, "to turn off before");
    u8g2_DrawStr(&_disp, 0, 62, "disconnecting power");

    u8g2_SendBuffer(&_disp);

    emit shutdownDisplayed();
}

void CFontz12864::fatalShutdownMessage()
{
}

void CFontz12864::setupGameDisplay()
{
    u8g2_ClearBuffer(&_disp);
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Bet amount field
    u8g2_DrawStr(&_disp, 0, 49, "Bet");

    // Credits field
    u8g2_DrawStr(&_disp, 75, 49, "Cred");

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showCreditsInGame(quint32 nbPlayerCred)
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Credit count
    u8g2_DrawStr(&_disp, 99, 49, QString::number(nbPlayerCred).rightJustified(5, ' ').toUtf8());

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showWinnings(const QString &winString, quint32 winCredits)
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Winning amount
    if (winCredits != 0) {
        u8g2_DrawStr(&_disp, 1, 38,
                     (winString + " +" + QString::number(winCredits).leftJustified(21, ' ')).toUtf8());
    } else {
        u8g2_DrawStr(&_disp, 1, 38, winString.leftJustified(21, ' ').toUtf8());
    }

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showBetAmount(quint32 creditsBet)
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Credit count
    u8g2_DrawStr(&_disp, 18, 49, QString::number(creditsBet).rightJustified(2, ' ').toUtf8());

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showCardValue(int cardIdx, PlayingCard cardToShow)
{
    // Clear the requested card position before rendering
    u8g2_SetDrawColor(&_disp, 0);

    if (cardIdx == 0) {
        u8g2_DrawBox(&_disp,   7, 0, 17, 25);
    } else if (cardIdx == 1) {
        u8g2_DrawBox(&_disp,  31, 0, 17, 25);
    } else if (cardIdx == 2) {
        u8g2_DrawBox(&_disp,  55, 0, 17, 25);
    } else if (cardIdx == 3) {
        u8g2_DrawBox(&_disp,  79, 0, 17, 25);
    } else if (cardIdx == 4) {
        u8g2_DrawBox(&_disp, 103, 0, 17, 25);
    }

    // Draw the card details
    u8g2_SetDrawColor(&_disp, 1);

    // Render the card suit
    unsigned char *suitBitmap;
    switch (cardToShow.suit()) {
    case PlayingCard::CLUB:
        suitBitmap = _club_bitmap;
        break;
    case PlayingCard::SPADE:
        suitBitmap = _spade_bitmap;
        break;
    case PlayingCard::DIAMOND:
        suitBitmap = _diamond_bitmap;
        break;
    case PlayingCard::HEART:
        suitBitmap = _heart_bitmap;
        break;
    default:
        break;
    }

    // Render the card rank
    QString cardRank;
    switch (cardToShow.value()) {
    case PlayingCard::TWO:
        cardRank = "2";
        break;
    case PlayingCard::THREE:
        cardRank = "3";
        break;
    case PlayingCard::FOUR:
        cardRank = "4";
        break;
    case PlayingCard::FIVE:
        cardRank = "5";
        break;
    case PlayingCard::SIX:
        cardRank = "6";
        break;
    case PlayingCard::SEVEN:
        cardRank = "7";
        break;
    case PlayingCard::EIGHT:
        cardRank = "8";
        break;
    case PlayingCard::NINE:
        cardRank = "9";
        break;
    case PlayingCard::TEN:
        cardRank = "10";
        break;
    case PlayingCard::JACK:
        cardRank = "J";
        break;
    case PlayingCard::QUEEN:
        cardRank = "Q";
        break;
    case PlayingCard::KING:
        cardRank = "K";
        break;
    case PlayingCard::ACE:
        cardRank = "A";
        break;
    default:
        break;
    }

    // Use a bigger font for cards
    u8g2_SetFont(&_disp, u8g2_font_7x13_mf);

    if (cardIdx == 0) {
        u8g2_DrawFrame(&_disp,   7, 0, 17, 25);
        u8g2_DrawXBM(&_disp, 11, 14, 9, 9, suitBitmap);
        if (cardRank != "10") {
            u8g2_DrawStr(&_disp, 12, 12, cardRank.toUtf8());
        } else {
            u8g2_DrawStr(&_disp,  9, 12, cardRank.toUtf8());
        }
    } else if (cardIdx == 1) {
        u8g2_DrawFrame(&_disp,  31, 0, 17, 25);
        u8g2_DrawXBM(&_disp, 35, 14, 9, 9, suitBitmap);
        if (cardRank != "10") {
            u8g2_DrawStr(&_disp, 36, 12, cardRank.toUtf8());
        } else {
            u8g2_DrawStr(&_disp, 33, 12, cardRank.toUtf8());
        }
    } else if (cardIdx == 2) {
        u8g2_DrawFrame(&_disp,  55, 0, 17, 25);
        u8g2_DrawXBM(&_disp, 59, 14, 9, 9, suitBitmap);
        if (cardRank != "10") {
            u8g2_DrawStr(&_disp, 60, 12, cardRank.toUtf8());
        } else {
            u8g2_DrawStr(&_disp, 57, 12, cardRank.toUtf8());
        }
    } else if (cardIdx == 3) {
        u8g2_DrawFrame(&_disp,  79, 0, 17, 25);
        u8g2_DrawXBM(&_disp, 83, 14, 9, 9, suitBitmap);
        if (cardRank != "10") {
            u8g2_DrawStr(&_disp, 84, 12, cardRank.toUtf8());
        } else {
            u8g2_DrawStr(&_disp, 81, 12, cardRank.toUtf8());
        }
    } else if (cardIdx == 4) {
        u8g2_DrawFrame(&_disp, 103, 0, 17, 25);
        u8g2_DrawXBM(&_disp, 107, 14, 9, 9, suitBitmap);
        if (cardRank != "10") {
            u8g2_DrawStr(&_disp, 108, 12, cardRank.toUtf8());
        } else {
            u8g2_DrawStr(&_disp, 105, 12, cardRank.toUtf8());
        }
    }

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showHoldIndicator(int cardIdx, bool isHeld)
{
    if (isHeld) {
        u8g2_SetDrawColor(&_disp, 1);
        if (cardIdx == 0) {
            u8g2_DrawBox(&_disp,   6, 26, 19, 2);
        } else if (cardIdx == 1) {
            u8g2_DrawBox(&_disp,  30, 26, 19, 2);
        } else if (cardIdx == 2) {
            u8g2_DrawBox(&_disp,  54, 26, 19, 2);
        } else if (cardIdx == 3) {
            u8g2_DrawBox(&_disp,  78, 26, 19, 2);
        } else if (cardIdx == 4) {
            u8g2_DrawBox(&_disp, 102, 26, 19, 2);
        }
    } else {
        u8g2_SetDrawColor(&_disp, 0);
        if (cardIdx == 0) {
            u8g2_DrawBox(&_disp,   6, 26, 19, 2);
        } else if (cardIdx == 1) {
            u8g2_DrawBox(&_disp,  30, 26, 19, 2);
        } else if (cardIdx == 2) {
            u8g2_DrawBox(&_disp,  54, 26, 19, 2);
        } else if (cardIdx == 3) {
            u8g2_DrawBox(&_disp,  78, 26, 19, 2);
        } else if (cardIdx == 4) {
            u8g2_DrawBox(&_disp, 102, 26, 19, 2);
        }
    }

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::showCardFrames(bool card1, bool card2, bool card3, bool card4, bool card5)
{
    u8g2_SetDrawColor(&_disp, 1);

    if (card1)
        u8g2_DrawBox(&_disp,   7, 0, 17, 25);

    if (card2)
        u8g2_DrawBox(&_disp,  31, 0, 17, 25);

    if (card3)
        u8g2_DrawBox(&_disp,  55, 0, 17, 25);

    if (card4)
        u8g2_DrawBox(&_disp,  79, 0, 17, 25);

    if (card5)
        u8g2_DrawBox(&_disp, 103, 0, 17, 25);

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::displayNoFundsWarning()
{
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Credit count
    u8g2_DrawStr(&_disp, 1, 38, "Insufficient Credits!");

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::clearAllHolds()
{
    // Overwrites all the hold indicators in a single push since SPI is sloooooooowwwwwwwwwwwwwww ;-)
    u8g2_SetDrawColor(&_disp, 0);
    u8g2_DrawBox(&_disp, 6, 26, 115, 2);
    u8g2_SendBuffer(&_disp);
}

void CFontz12864::setupPayTableDisplay(const QString &gameName)
{
    u8g2_ClearBuffer(&_disp);
    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_6x10_mf);

    // Title Bar Text and Line
    u8g2_DrawStr(&_disp, 0, 8, gameName.toUtf8());
    u8g2_DrawHLine(&_disp,  0, 11, 128);

    u8g2_SendBuffer(&_disp);
}

void CFontz12864::displayTablePage(QVector<QPair<const QString, int> > table, int startIdx, int nbItems)
{
    // Clear the display table area
    u8g2_SetDrawColor(&_disp, 0);
    u8g2_DrawBox(&_disp, 0, 12, 128, 40);

    u8g2_SetFontMode(&_disp, 0);
    u8g2_SetFontDirection(&_disp, 0);
    u8g2_SetDrawColor(&_disp, 1);
    u8g2_SetFont(&_disp, u8g2_font_5x8_mf);

    // Item Loop
    int vertPixel = 21;
    for (int tblIdx = startIdx; tblIdx < table.size() && tblIdx < nbItems; ++tblIdx, vertPixel += 9) {
        u8g2_DrawStr(&_disp, 0, vertPixel, table[tblIdx].first.toUtf8());
        u8g2_DrawStr(&_disp, 109, vertPixel, QString::number(table[tblIdx].second).rightJustified(4, ' ').toUtf8());
    }

    u8g2_SendBuffer(&_disp);
}
