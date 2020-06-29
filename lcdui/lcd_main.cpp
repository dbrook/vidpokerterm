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
#include "cfontz12864.h"
#include "consolekeyboardinput.h"
#include "gameaccountinterface.h"
#include "raspigpioinput.h"

#include <QCommandLineParser>
#include <QCoreApplication>

#include <QThread>
#include <QObject>

#include <wiringPi.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption useKeyboardNotGPIO(QStringList() << "k",
                                       QCoreApplication::translate("main", "Use standard keyboard, not GPIO buttons."));
    parser.addOption(useKeyboardNotGPIO);
    parser.process(a);

    bool useKeyboard = false;
    if (parser.isSet(useKeyboardNotGPIO)) {
        useKeyboard = true;
    }

    // Needed for Crystalfontz12864 interaction (due to SPI pin setup) and GPIO pin event processing
    wiringPiSetup();

    // Shutdown indicator
    bool haltAtNextInteraction = false;

    // LCD Display Thread
//    CFontz634 *display        = new CFontz634("/dev/ttyUSB0");
    CFontz12864 *display = new CFontz12864;
    QThread   *displayHandler = new QThread;
    display->setupWelcomeDisplay();
    display->moveToThread(displayHandler);
    displayHandler->start();

    GenericInputHandler *inputs;
    if (useKeyboard) {
        // Keyboard Event Processing
        inputs = new ConsoleKeyboardInput(&haltAtNextInteraction);
    } else {
        // GPIO (on Raspberry Pi) Event Processing
        inputs = new RasPiGPIOInput(&haltAtNextInteraction);
    }
    QThread *inputHandler = new QThread;
    QObject::connect(inputHandler,  &QThread::started, inputs, &GenericInputHandler::watch);
    inputHandler->start();

    // Fire off the application + provide a quit connection
    GameAccountInterface *account       = new GameAccountInterface(3, display, inputs, &haltAtNextInteraction);
    QThread              *acctInterface = new QThread;
    account->moveToThread(acctInterface);
    acctInterface->start();

    /*
     * All the event-loop-having threads need to be killed before a quit can be successfully processed!
     *
     * Why? Waiting for keypresses in a console application requires blocking calls to getchar() (along with other
     * Linux-only terminal trickery). The same principal applies to waiting for GPIO events using the wiringPi library.
     * Therefore, the input processor objects cannot know that they processed a "exit" keypress until told (which will
     * be the next blocking call), but when this happens they will already be blocking the thread on the next wait!
     *
     * To deal with this, exiting is a multi-stage process:
     *      1) GameAccountInterface tells the attached display that an exit was requested so it can display a message
     *      2) The "quit manual event loop" Boolean is enabled by the GameAccountInterface
     *      3) The input event handler has already blocked at the next GPIO / keypress wait
     *      4) After the next input event is triggered, it will see that the "quit manual event loop" is on and break
     *          the infinite loop. This will give control back to the Qt event loop after emitting a "readyToStop"
     *      5) readyToStop is received by the attached display which can now say the shutdown is in progress
     *      6) After displaying this message, the display emits shutdownDisplayed to indicate that it was updated
     *      7) The input event processor can now be deleted when receiving the shutdownDisplayed signal
     *      8) The terminal is cleaned up and the game quits.
     */
    QObject::connect(account, &GameAccountInterface::haltRequested, display, &GenericLCD::showPreShutdownWarning);
    QObject::connect(inputs, &GenericInputHandler::readyToStop,
                     display, &GenericLCD::showShutdownMessage);
    QObject::connect(display, &GenericLCD::shutdownDisplayed,
                     inputs, &GenericInputHandler::deleteLater, Qt::QueuedConnection);
    QObject::connect(inputs, &GenericInputHandler::destroyed, &a, &QCoreApplication::quit, Qt::QueuedConnection);

    return a.exec();
}
