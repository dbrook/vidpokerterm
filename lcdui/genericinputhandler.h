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

#ifndef GENERICINPUTHANDLER_H
#define GENERICINPUTHANDLER_H

#include <QObject>

/**
 * @brief The GenericInputHandler class is an abstract base class to handle any inputs sent in (from say a keyboard, or
 *        Raspberry Pi GPIO pins, etc.) and send these events via Qt signal emission to an intermediary which handles
 *        communication with a PokerGame and display.
 *
 * @note  This is a low-level interaction model for when a GUI is not available. There are 10 accepted inputs to the
 *        VidPokerTerm application:
 *        - 5x hold buttons which link to a specific card to indicate the desire to prevent that card from being redrawn
 *        - 4x dynamic soft-keys whose function will be dependent on the active part of the application
 *        - 1x trigger or select button, which, in a game will be the main deal/draw button
 *
 * @note  This should be put into its own QThread using moveToThread so that sign
 */
class GenericInputHandler : public QObject
{
    Q_OBJECT
public:
    explicit GenericInputHandler(QObject *parent = nullptr);

    virtual ~GenericInputHandler();

    /**
     * @brief watch should be overridden in a child class and contain an infinite loop that will wait for events on the
     *        desired inputs and then emit them as appropriate signals (declared below) when discovered
     */
    virtual void watch() = 0;

signals:
    /**
     * @brief softkeyPressed is emitted when the input handler detects a softkey was pressed
     *
     * @param position            Which softkey was pressed (0, 1, 2, 3)
     */
    void softkeyPressed(int position);

    /**
     * @brief holdPressed is emitted when the input handler detects a card-hold-button was pressed
     *
     * @param position            Which hold key was pressed (1, 2, 3, 4, 5)
     */
    void holdPressed(int position);

    /**
     * @brief triggerPressed is emitted when the main trigger/select/deal/draw button was pressed
     */
    void triggerPressed();

    /**
     * @brief readyToStop is emitted when the loop with blocking getchar() calls is terminated
     */
    void readyToStop();
};

#endif // GENERICINPUTHANDLER_H
