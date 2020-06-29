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

#ifndef RASPIGPIOINPUT_H
#define RASPIGPIOINPUT_H

#include "genericinputhandler.h"

#include <QObject>

/**
 * @brief The RasPiGPIOInput class sets Raspberry Pi GPIO pins as receive pins to be able to watch for button presses.
 *
 *        5 hold keys (1 for each poker hand card) are supported:
 *          Hold Card 1:    Pin 37    WiringPi Pin 25    BCM Pin 26
 *          Hold Card 2:    Pin 36    WiringPi Pin 27    BCM Pin 16
 *          Hold Card 3:    Pin 33    WiringPi Pin 23    BCM Pin 13
 *          Hold Card 4:    Pin 32    WiringPi Pin 26    BCM Pin 12
 *          Hold Card 5:    Pin 22    WiringPi Pin  6    BCM Pin 25
 *
 *        4 softkeys (value shown at bottom of LCD interface)
 *          Soft Key  1:    Pin 16    WiringPi Pin  4    BCM Pin 23
 *          Soft Key  2:    Pin 15    WiringPi Pin  3    BCM Pin 22
 *          Soft Key  3:    Pin 13    WiringPi Pin  2    BCM Pin 27
 *          Soft Key  4:    Pin 12    WiringPi Pin  1    BCM Pin 18
 *
 *        Trigger/Select:   Pin 11    WiringPi Pin  0    BCM Pin 17
 *
 * @warning in order to use these inputs, the wiringPiSetup() function must be run prior to instantiating an object
 */
class RasPiGPIOInput : public GenericInputHandler
{
    Q_OBJECT
public:
    explicit RasPiGPIOInput(bool *haltAtNextPress, QObject *parent = nullptr);

    ~RasPiGPIOInput();

    /**
     * @brief Processes the GPIO events and emit them as VidPokerTerm-compatible signals. Relies on blocking calls,
     *        to the quitManualEventLoop should be tracked/set by the user interface class(es) to know when to exit.
     */
    virtual void watch();

private:
    // Card hold keys
    static void holdKey1(void);
    static void holdKey2(void);
    static void holdKey3(void);
    static void holdKey4(void);
    static void holdKey5(void);

    // Softkey buttons
    static void softkey1(void);
    static void softkey2(void);
    static void softkey3(void);
    static void softkey4(void);

    // Trigger/Deal/Draw/Select button
    static void triggerKey(void);

    // Track the last press times for software debouncing
    static qint64 _lastHoldKey1;
    static qint64 _lastHoldKey2;
    static qint64 _lastHoldKey3;
    static qint64 _lastHoldKey4;
    static qint64 _lastHoldKey5;

    static qint64 _lastSoftkey1;
    static qint64 _lastSoftkey2;
    static qint64 _lastSoftkey3;
    static qint64 _lastSoftkey4;

    static qint64 _lastTriggerKey;

private:
    bool *_quitManualEventLoop;

    // Pointer to the instance so events can be processed
    static RasPiGPIOInput *s_inst;
};

#endif // RASPIGPIOINPUT_H
