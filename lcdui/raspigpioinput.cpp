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

#include "raspigpioinput.h"

#include <wiringPi.h>

#include <QDebug>
#include <QDateTime>

RasPiGPIOInput* RasPiGPIOInput::s_inst = nullptr;

qint64 RasPiGPIOInput::_lastHoldKey1 = 0;
qint64 RasPiGPIOInput::_lastHoldKey2 = 0;
qint64 RasPiGPIOInput::_lastHoldKey3 = 0;
qint64 RasPiGPIOInput::_lastHoldKey4 = 0;
qint64 RasPiGPIOInput::_lastHoldKey5 = 0;

qint64 RasPiGPIOInput::_lastSoftkey1 = 0;
qint64 RasPiGPIOInput::_lastSoftkey2 = 0;
qint64 RasPiGPIOInput::_lastSoftkey3 = 0;
qint64 RasPiGPIOInput::_lastSoftkey4 = 0;

qint64 RasPiGPIOInput::_lastTriggerKey = 0;

RasPiGPIOInput::RasPiGPIOInput(bool *haltAtNextPress, QObject *parent)
    : GenericInputHandler (parent),
      _quitManualEventLoop(haltAtNextPress)
{
    // Keep pointer to the input processor in static space so event callbacks can actually be emitted
    s_inst = this;

    // Mark all the keypress pins as input pins
    // Hold Keys
    pinMode(25, INPUT);
    pullUpDnControl(25, PUD_DOWN);
    pinMode(27, INPUT);
    pullUpDnControl(27, PUD_DOWN);
    pinMode(23, INPUT);
    pullUpDnControl(23, PUD_DOWN);
    pinMode(26, INPUT);
    pullUpDnControl(26, PUD_DOWN);
    pinMode( 6, INPUT);
    pullUpDnControl(6, PUD_DOWN);

    // Soft Keys
    pinMode( 4, INPUT);
    pullUpDnControl(4, PUD_DOWN);
    pinMode( 3, INPUT);
    pullUpDnControl(3, PUD_DOWN);
    pinMode( 2, INPUT);
    pullUpDnControl(2, PUD_DOWN);
    pinMode( 1, INPUT);
    pullUpDnControl(1, PUD_DOWN);

    // Select/Trigger/Deal/Draw Key
    pinMode( 0, INPUT);
    pullUpDnControl(0, PUD_DOWN);

    // Track button push event times in milliseconds-since-epoch to debounce
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    _lastHoldKey1 = current;
    _lastHoldKey2 = current;
    _lastHoldKey3 = current;
    _lastHoldKey4 = current;
    _lastHoldKey5 = current;

    _lastSoftkey1 = current;
    _lastSoftkey2 = current;
    _lastSoftkey3 = current;
    _lastSoftkey4 = current;

    _lastTriggerKey = current;
}

RasPiGPIOInput::~RasPiGPIOInput()
{

}

void RasPiGPIOInput::watch()
{
    wiringPiISR(25, INT_EDGE_RISING, RasPiGPIOInput::holdKey1);
    wiringPiISR(27, INT_EDGE_RISING, RasPiGPIOInput::holdKey2);
    wiringPiISR(23, INT_EDGE_RISING, RasPiGPIOInput::holdKey3);
    wiringPiISR(26, INT_EDGE_RISING, RasPiGPIOInput::holdKey4);
    wiringPiISR( 6, INT_EDGE_RISING, RasPiGPIOInput::holdKey5);

    wiringPiISR( 4, INT_EDGE_RISING, RasPiGPIOInput::softkey1);
    wiringPiISR( 3, INT_EDGE_RISING, RasPiGPIOInput::softkey2);
    wiringPiISR( 2, INT_EDGE_RISING, RasPiGPIOInput::softkey3);
    wiringPiISR( 1, INT_EDGE_RISING, RasPiGPIOInput::softkey4);

    wiringPiISR( 0, INT_EDGE_RISING, RasPiGPIOInput::triggerKey);
}

void RasPiGPIOInput::holdKey1()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastHoldKey1 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->holdPressed(0);
    }

    _lastHoldKey1 = current;
}

void RasPiGPIOInput::holdKey2()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastHoldKey2 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->holdPressed(1);
    }

    _lastHoldKey2 = current;
}

void RasPiGPIOInput::holdKey3()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastHoldKey3 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->holdPressed(2);
    }

    _lastHoldKey3 = current;
}

void RasPiGPIOInput::holdKey4()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastHoldKey4 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->holdPressed(3);
    }

    _lastHoldKey4 = current;
}

void RasPiGPIOInput::holdKey5()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastHoldKey5 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->holdPressed(4);
    }

    _lastHoldKey5 = current;
}

void RasPiGPIOInput::softkey1()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastSoftkey1 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->softkeyPressed(0);
    }

    _lastSoftkey1 = current;
}

void RasPiGPIOInput::softkey2()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastSoftkey2 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->softkeyPressed(1);
    }

    _lastSoftkey2 = current;
}

void RasPiGPIOInput::softkey3()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastSoftkey3 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->softkeyPressed(2);
    }

    _lastSoftkey3 = current;
}

void RasPiGPIOInput::softkey4()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastSoftkey4 > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->softkeyPressed(3);
    }

    _lastSoftkey4 = current;
}

void RasPiGPIOInput::triggerKey()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();

    if (current - _lastTriggerKey > 300) {
        if (*(s_inst->_quitManualEventLoop)) {
            qDebug() << "Shutdown will commence...";
            emit s_inst->readyToStop();
        }
        emit s_inst->triggerPressed();
    }

    _lastTriggerKey = current;
}
