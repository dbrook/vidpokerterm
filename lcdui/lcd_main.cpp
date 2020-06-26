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
#include "consolekeyboardinput.h"
#include "gameaccountinterface.h"

#include <QCoreApplication>

#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Shutdown indicator
    bool haltAtNextInteraction = false;

    // LCD Display Thread
    CFontz634 *display        = new CFontz634("/dev/ttyUSB0");
    QThread   *displayHandler = new QThread;
    display->setupWelcomeDisplay();
    display->moveToThread(displayHandler);
    displayHandler->start();

    // Keyboard event processing
    ConsoleKeyboardInput *keyEventLogic = new ConsoleKeyboardInput(&haltAtNextInteraction);
    QThread              *inputHandler  = new QThread;
    QObject::connect(inputHandler,  &QThread::started, keyEventLogic, &GenericInputHandler::watch);
    inputHandler->start();

    // Fire off the application + provide a quit connection
    GameAccountInterface *account       = new GameAccountInterface(4, display, keyEventLogic, &haltAtNextInteraction);
    QThread              *acctInterface = new QThread;
    account->moveToThread(acctInterface);
    acctInterface->start();

    // All the event-loop-having threads need to be killed before a quit can be successfully processed!
    // The GenericInputHandler uses blocking calls to deal with inputs, so that loop must be signaled to be killed
    // Do NOT mess with the ordering and QueuedConnection flags here, this flow has been shown to work (at least with
    // the keypress event handling loops/escapes). It's MOSTLY thread safe...? ;-)
    QObject::connect(account,       &GameAccountInterface::haltRequested,
                     display,       &GenericLCD::showPreShutdownWarning);
    QObject::connect(keyEventLogic, &GenericInputHandler::readyToStop,
                     display,       &GenericLCD::deleteLater, Qt::QueuedConnection);
    QObject::connect(keyEventLogic, &GenericInputHandler::readyToStop,
                     keyEventLogic, &GenericInputHandler::deleteLater, Qt::QueuedConnection);
    QObject::connect(keyEventLogic, &GenericInputHandler::destroyed, &a, &QCoreApplication::quit, Qt::QueuedConnection);

    return a.exec();
}
