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

#include "consolekeyboardinput.h"

#include <QDebug>

/*
 * This little trick was adapted [somewhat] from here:
 * https://stackoverflow.com/questions/7543313/how-to-handle-keypress-events-in-a-qt-console-application
 */
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

static struct termios originalTerminalSettings;
static struct termios eventProcessingTermSettings;

ConsoleKeyboardInput::ConsoleKeyboardInput(bool *haltAtNextPress, QObject *parent)
    : GenericInputHandler (parent),
      _quitManualEventLoop(haltAtNextPress)
{
    // Initialize terminal to be able to read characters and not line-buffered input
    tcgetattr(0, &originalTerminalSettings);                 /* grab old terminal i/o settings */
    eventProcessingTermSettings = originalTerminalSettings;  /* make new settings same as old settings */
    eventProcessingTermSettings.c_lflag &= ~ICANON;          /* disable buffered i/o */
    eventProcessingTermSettings.c_lflag &= ~ECHO;            /* set echo mode to off */
    tcsetattr(0, TCSANOW, &eventProcessingTermSettings);     /* use these new terminal i/o settings now */

    qDebug() << "Attention: You are using the keyboard interaction interface of VidPokerTerm";
    qDebug() << "  Select/deal/draw . . . . [/]";
    qDebug() << "  Softkey #1 is  . . . . . [N]";
    qDebug() << "  Softkey #2 is  . . . . . [M]";
    qDebug() << "  Softkey #3 is  . . . . . [,]";
    qDebug() << "  Softkey #4 is  . . . . . [.]";
    qDebug() << "  Hold Card # 1 is . . . . [A]";
    qDebug() << "  Hold Card # 2 is . . . . [S]";
    qDebug() << "  Hold Card # 3 is . . . . [D]";
    qDebug() << "  Hold Card # 4 is . . . . [F]";
    qDebug() << "  Hold Card # 5 is . . . . [G]";
}

ConsoleKeyboardInput::~ConsoleKeyboardInput()
{
    // Restore the terminal to its original characteristics
    tcsetattr(0, TCSANOW, &originalTerminalSettings);

    qDebug() << "Deleting the key watcher, restoring original terminal attributes";
}

void ConsoleKeyboardInput::watch()
{
    while (1) {
        char key = getchar();

        if (*_quitManualEventLoop) {
            qDebug() << "Shutdown will commence...";
            emit readyToStop();
            break;
            // Control will be returned to the Qt event loop and not blocked here
        }

        // Determine which key was processed
        switch (key) {
        /*
         * Hold Buttons
         */
        case 'a':
        case 'A':
            emit holdPressed(0);
            break;
        case 's':
        case 'S':
            emit holdPressed(1);
            break;
        case 'd':
        case 'D':
            emit holdPressed(2);
            break;
        case 'f':
        case 'F':
            emit holdPressed(3);
            break;
        case 'g':
        case 'G':
            emit holdPressed(4);
            break;
        /*
         * Softkeys
         */
        case 'n':
        case 'N':
            emit softkeyPressed(0);
            break;
        case 'm':
        case 'M':
            emit softkeyPressed(1);
            break;
        case ',':
            emit softkeyPressed(2);
            break;
        case '.':
            emit softkeyPressed(3);
            break;

        /*
         * Trigger/select/deal/draw
         */
        case '/':
            emit triggerPressed();
            break;

        default:
            ;// Do nothing
        }
    }
}
