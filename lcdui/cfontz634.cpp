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

#include "cfontz634.h"

// Ancient Unix~y libraries for low-level serial port bashing!
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <QThread>

CFontz634::CFontz634(const QString &device, QObject *parent)
    : GenericLCD  (parent),
      _deviceName (device),
      _baudRate   (B19200)
{
    // Open the serial port ... yes, we're using the low-level C-style code from Crystalfontz as inspiration
    _deviceHandle = open(_deviceName.toUtf8(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Unable to open the screen at " + device.toUtf8());
    }

    // Ensure the rate is doable
    struct termios term;

    // Get device struct
    if (tcgetattr(_deviceHandle, &term) != 0) {
        throw std::runtime_error("Unable to retrieve serial device structure");
    }

    // Input modes
    term.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IXOFF);
    term.c_iflag |=   IGNPAR;

    // Output modes
    term.c_oflag &= ~(OPOST|ONLCR|OCRNL|ONOCR|ONLRET|OFILL|OFDEL|NLDLY|CRDLY|TABDLY|BSDLY|VTDLY|FFDLY);

    // Control modes
    term.c_cflag &= ~(CSIZE|PARENB|PARODD|HUPCL|CRTSCTS);
    term.c_cflag |=   CREAD|CS8|CSTOPB|CLOCAL;

    // Local modes
    term.c_lflag &= ~(ISIG|ICANON|IEXTEN|ECHO);
    term.c_lflag |=   NOFLSH;

    // Set baud rate
    cfsetospeed(&term, _baudRate);
    cfsetispeed(&term, _baudRate);

    // Set new device settings
    if (tcsetattr(_deviceHandle, TCSANOW, &term) != 0) {
        throw std::runtime_error("Failure in tcsetattr");
    }

    // Wait a few seconds (CFA634 (at least the serial one) boots up and shows a branding screen which then disappears)
    QThread::sleep(3);

    // Disable wrapping, scrolling
    this->sendByte(20);
    this->sendByte(24);

    // Set the cursor to stop blinking
    this->sendByte(4);

    // Setup any custom characters
    this->setupCharacters();

    // Clear the display
    this->clearDisplay();
}

CFontz634::~CFontz634()
{
    close(_deviceHandle);
    _deviceHandle = 0;
}

void CFontz634::setupCharacters()
{
    // A Crystalfontz 634 screen has 8 CGRAM character positions we can save custom stuff into
    // Let's assign CGRAM[0] to a "10" as a single character, CGRAM[1]:♦, CGRAM[2]:♣, CGRAM[3]:♥, CGRAM[4]:♠

    // 10
    this->sendByte(25);   // SetCustomChar
    this->sendByte(0);    // CustomChar (CGRAM) Position
    this->sendByte(18);   // Row / Data 0
    this->sendByte(21);   // Row / Data 1
    this->sendByte(21);   // Row / Data 2
    this->sendByte(21);   // Row / Data 3
    this->sendByte(21);   // Row / Data 4
    this->sendByte(21);   // Row / Data 5
    this->sendByte(18);   // Row / Data 6
    this->sendByte( 0);   // Row / Data 7

    // Diamond is actually already mapped as CGROM character 148
    // Heart is actually already mapped as CGROM character 147

    // Club
    this->sendByte(25);   // SetCustomChar
    this->sendByte(1);    // CustomChar (CGRAM) Position
    this->sendByte( 0);   // Row / Data 0
    this->sendByte(14);   // Row / Data 1
    this->sendByte(31);   // Row / Data 2
    this->sendByte(31);   // Row / Data 3
    this->sendByte( 4);   // Row / Data 4
    this->sendByte(14);   // Row / Data 5
    this->sendByte( 0);   // Row / Data 6
    this->sendByte( 0);   // Row / Data 7

    // Spade
    this->sendByte(25);   // SetCustomChar
    this->sendByte(2);    // CustomChar (CGRAM) Position
    this->sendByte( 0);   // Row / Data 0
    this->sendByte( 4);   // Row / Data 1
    this->sendByte(14);   // Row / Data 2
    this->sendByte(31);   // Row / Data 3
    this->sendByte(21);   // Row / Data 4
    this->sendByte(14);   // Row / Data 5
    this->sendByte( 0);   // Row / Data 6
    this->sendByte( 0);   // Row / Data 7
}

void CFontz634::clearDisplay()
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->sendByte(12);
}

void CFontz634::cursorToTextPosition(int row, int col)
{
    // Set cursor position - command byte 17
    this->sendByte(17);
    this->sendByte(col);
    this->sendByte(row);
}

void CFontz634::writeTextAtPos(int startRow, int startCol, const QString &text)
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->cursorToTextPosition(startRow, startCol);
    this->sendNullTermBytes(text.toUtf8());
}

void CFontz634::writeByteAtPos(int startRow, int startCol, const unsigned char lcdChar)
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->cursorToTextPosition(startRow, startCol);
    this->sendByte(lcdChar);
}

void CFontz634::fillSoftkeys(QVector <QString> softkeys)
{
    this->writeTextAtPos(3,  0, softkeys[0].leftJustified(4, ' '));
    this->writeTextAtPos(3,  5, softkeys[1].leftJustified(4, ' '));
    this->writeTextAtPos(3, 10, softkeys[2].leftJustified(4, ' '));
    this->writeTextAtPos(3, 15, softkeys[3].leftJustified(5, ' '));
}

void CFontz634::setupWelcomeDisplay()
{
    this->writeTextAtPos(0, 0, "  VidPokerTerm LCD  ");
    this->writeTextAtPos(1, 0, "Cred:");
    this->writeTextAtPos(2, 0, "Game:");
}

void CFontz634::showCreditsInMainWin(quint32 nbPlayerCred)
{
    this->writeTextAtPos(1, 5, QString::number(nbPlayerCred).rightJustified(15, ' '));
}

void CFontz634::showGameName(const QString &gameName)
{
    // Can only show 15 characters
    this->writeTextAtPos(2, 5, gameName.leftJustified(15, ' '));
}

void CFontz634::showPreShutdownWarning()
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->sendByte(12);
    this->writeTextAtPos(0, 0, "Press a key to");
    this->writeTextAtPos(1, 0, "continue shutdown");
}

void CFontz634::showShutdownMessage()
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->clearDisplay();
    this->writeTextAtPos(3, 0, "Shutting down...");
    emit shutdownDisplayed();
}

void CFontz634::fatalShutdownMessage()
{
    if (_deviceHandle <= 0) {
        throw std::runtime_error("Display not ready / opened");
    }

    this->sendByte(12);
    this->writeTextAtPos(2, 0, "Applicaton crashed!");
}

void CFontz634::setupGameDisplay()
{
    this->sendByte(12);
    this->writeTextAtPos(0, 12, "Crd");
    this->writeTextAtPos(1, 12, "Bet");
}

void CFontz634::showCreditsInGame(quint32 nbPlayerCred)
{
    this->writeTextAtPos(0, 15, QString::number(nbPlayerCred).rightJustified(5, ' '));
}

void CFontz634::showWinnings(const QString &winString, quint32 winCredits)
{
    this->writeTextAtPos(2, 0, winString.leftJustified(15, ' '));
    if (winCredits != 0) {
        this->writeTextAtPos(2, 15, "+" + QString::number(winCredits).rightJustified(4, ' '));
    } else {
        this->writeTextAtPos(2, 15, "     ");
    }
}

void CFontz634::showBetAmount(quint32 creditsBet)
{
    this->writeTextAtPos(1, 15, QString::number(creditsBet).rightJustified(5, ' '));
}

void CFontz634::showCardValue(int cardIdx, PlayingCard cardToShow)
{
    PlayingCard::CardSuit suit = cardToShow.suit();
    PlayingCard::CardValue val = cardToShow.value();

    // Render the card as actual readable text and not the enum values they're based off of
    unsigned char valueSymbol = ' ';
    unsigned char cardSuit    = ' ';

    // Opted to use 4 different colors for the 4 suits. I saw a platform use this and I like it for identifying flushes
    switch (suit) {
    case PlayingCard::DIAMOND:
        cardSuit = 148;
        break;
    case PlayingCard::CLUB:
        cardSuit = 129;
        break;
    case PlayingCard::HEART:
        cardSuit = 147;
        break;
    case PlayingCard::SPADE:
        cardSuit = 130;
        break;
    default:
        // should not happen!
        break;
    }

    switch (val) {
    case PlayingCard::TWO:
        valueSymbol = '2';
        break;
    case PlayingCard::THREE:
        valueSymbol = '3';
        break;
    case PlayingCard::FOUR:
        valueSymbol = '4';
        break;
    case PlayingCard::FIVE:
        valueSymbol = '5';
        break;
    case PlayingCard::SIX:
        valueSymbol = '6';
        break;
    case PlayingCard::SEVEN:
        valueSymbol = '7';
        break;
    case PlayingCard::EIGHT:
        valueSymbol = '8';
        break;
    case PlayingCard::NINE:
        valueSymbol = '9';
        break;
    case PlayingCard::TEN:
        valueSymbol = 128;  // Custom "10" character
        break;
    case PlayingCard::JACK:
        valueSymbol = 'J';
        break;
    case PlayingCard::QUEEN:
        valueSymbol = 'Q';
        break;
    case PlayingCard::KING:
        valueSymbol = 'K';
        break;
    case PlayingCard::ACE:
        valueSymbol = 'A';
        break;
    default:
        // should not happen!
        break;
    }

    // Pick the right card to render, "flip it over" (turn it white), then render the card text
    switch (cardIdx) {
    case 0:
        this->writeByteAtPos(0, 0, valueSymbol);
        this->writeByteAtPos(1, 0, cardSuit);
        break;
    case 1:
        this->writeByteAtPos(0, 2, valueSymbol);
        this->writeByteAtPos(1, 2, cardSuit);
        break;
    case 2:
        this->writeByteAtPos(0, 4, valueSymbol);
        this->writeByteAtPos(1, 4, cardSuit);
        break;
    case 3:
        this->writeByteAtPos(0, 6, valueSymbol);
        this->writeByteAtPos(1, 6, cardSuit);
        break;
    case 4:
        this->writeByteAtPos(0, 8, valueSymbol);
        this->writeByteAtPos(1, 8, cardSuit);
        break;
    default:
        // Should not happen!
        break;
    }
}

void CFontz634::showHoldIndicator(int cardIdx, bool isHeld)
{
    switch (cardIdx) {
    case 0:
        if (isHeld) {
            cursorToTextPosition(0, 1);
            this->sendByte(218);
            cursorToTextPosition(1, 1);
            this->sendByte(218);
        } else {
            cursorToTextPosition(0, 1);
            this->sendByte(32);
            cursorToTextPosition(1, 1);
            this->sendByte(32);
        }
        break;
    case 1:
        if (isHeld) {
            cursorToTextPosition(0, 3);
            this->sendByte(218);
            cursorToTextPosition(1, 3);
            this->sendByte(218);
        } else {
            cursorToTextPosition(0, 3);
            this->sendByte(32);
            cursorToTextPosition(1, 3);
            this->sendByte(32);
        }
        break;
    case 2:
        if (isHeld) {
            cursorToTextPosition(0, 5);
            this->sendByte(218);
            cursorToTextPosition(1, 5);
            this->sendByte(218);
        } else {
            cursorToTextPosition(0, 5);
            this->sendByte(32);
            cursorToTextPosition(1, 5);
            this->sendByte(32);
        }
        break;
    case 3:
        if (isHeld) {
            cursorToTextPosition(0, 7);
            this->sendByte(218);
            cursorToTextPosition(1, 7);
            this->sendByte(218);
        } else {
            cursorToTextPosition(0, 7);
            this->sendByte(32);
            cursorToTextPosition(1, 7);
            this->sendByte(32);
        }
        break;
    case 4:
        if (isHeld) {
            cursorToTextPosition(0, 9);
            this->sendByte(218);
            cursorToTextPosition(1, 9);
            this->sendByte(218);
        } else {
            cursorToTextPosition(0, 9);
            this->sendByte(32);
            cursorToTextPosition(1, 9);
            this->sendByte(32);
        }
        break;
    default:
        ;
    }
}

void CFontz634::showCardFrames(bool card1, bool card2, bool card3, bool card4, bool card5)
{
    ; // Do nothing ... the CFA634 does not really have good enough graphic abilities to render blank cards
}

void CFontz634::displayNoFundsWarning()
{
    this->writeTextAtPos(2, 0, "Insufficient Funds!");
}

void CFontz634::clearAllHolds()
{
    cursorToTextPosition(0, 1);
    this->sendByte(32);
    cursorToTextPosition(1, 1);
    this->sendByte(32);
    cursorToTextPosition(0, 3);
    this->sendByte(32);
    cursorToTextPosition(1, 3);
    this->sendByte(32);
    cursorToTextPosition(0, 5);
    this->sendByte(32);
    cursorToTextPosition(1, 5);
    this->sendByte(32);
    cursorToTextPosition(0, 7);
    this->sendByte(32);
    cursorToTextPosition(1, 7);
    this->sendByte(32);
    cursorToTextPosition(0, 9);
    this->sendByte(32);
    cursorToTextPosition(1, 9);
    this->sendByte(32);
}

void CFontz634::setupPayTableDisplay(const QString &gameName)
{
    // TODO: implement later
    ;
}

void CFontz634::displayTablePage(QVector<QPair<const QString, int> > table, int startIdx, int nbItems)
{
    // TODO: implement later
    ;
}

void CFontz634::sendByte(unsigned char byte)
{
    write(_deviceHandle, &byte, 1);
}

void CFontz634::sendNullTermBytes(const char *data)
{
    // So long as a NULL terminator hasn't been found, render whatever is next in the data array
    while (*data) {
        this->sendByte(*data++);
    }
}
