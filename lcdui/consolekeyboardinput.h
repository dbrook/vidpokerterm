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

#ifndef CONSOLEKEYBOARDINPUT_H
#define CONSOLEKEYBOARDINPUT_H

#include "genericinputhandler.h"

/**
 * @brief The ConsoleKeyboardInput class processes keypress events and converts them into VidPokerTerm signals. This is
 *        mostly for testing, and is biased towards a US-ANSI keyboard layout:
 *          A S D F G                         N M , . /
 *          | | | | +-- Hold card #5          | | | | +-- Trigger / Select / Deal / Draw
 *          | | | +---- Hold card #4          | | | +---- Softkey #3
 *          | | +------ Hold card #3          | | +------ Softkey #2
 *          | +-------- Hold card #2          | +-------- Softkey #1
 *          +---------- Hold card #1          +---------- Softkey #0
 *
 * @note  This is extremely un-portable code, relying on POSIX terminal library trickery to watch for keypresses
 */
class ConsoleKeyboardInput : public GenericInputHandler
{
    Q_OBJECT
public:
    explicit ConsoleKeyboardInput(bool *haltAtNextPress, QObject *parent = nullptr);

    ~ConsoleKeyboardInput();

    /**
     * @brief Processes the keyboard events and emits them as VidPokerTerm-compatible signals
     */
    virtual void watch();

private:
    bool *_quitManualEventLoop;
};

#endif // CONSOLEKEYBOARDINPUT_H
