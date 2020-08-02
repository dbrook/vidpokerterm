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

#include "paytableinterface.h"

#include <QDebug>

PayTableInterface::PayTableInterface(int                  nbSoftkeys,
                                     GenericLCD          *lcdScreen,
                                     GenericInputHandler *inputs,
                                     GameOrchestrator    *orchestrator,
                                     const QString       &gameName,
                                     QObject             *parent)
    : LCDInterface(nbSoftkeys, parent),
      _lcd        (lcdScreen),
      _input      (inputs),
      _synchroOrc (orchestrator),
      _gameName   (gameName),
      _currentPage(0)
{
    // TODO Type registering?
    qRegisterMetaType<QVector <QPair <const QString, int> > >("QVector<QPair<const QString, int>>");

    // TODO Initialize Display


    // TODO Setup Softkeys
    this->addSoftkeyFunction("PgUp  ", static_cast<void (LCDInterface::*)()>(&PayTableInterface::pageUp));
    this->addSoftkeyFunction("PgDown", static_cast<void (LCDInterface::*)()>(&PayTableInterface::pageDown));
    this->addSoftkeyFunction("Return", static_cast<void (LCDInterface::*)()>(&PayTableInterface::closeTable));
    this->finishSoftkeys();

    // Capture the paytable from the orchestrator and hold it locally
    orchestrator->currentPayTable(_payTable);

    // Initialize display and connections
    this->restoreConnections();
}

PayTableInterface::~PayTableInterface()
{
    // TODO Disconnect anything


    qDebug() << "Closing the paytable display";
}

void PayTableInterface::displayCurrentPage()
{
    emit showTableRange(_payTable, _currentPage * 4, _currentPage * 4 + 4);
}

void PayTableInterface::pageDown()
{
    if ((_currentPage +1) * 4 < _payTable.size()) {
        ++_currentPage;
        this->displayCurrentPage();
    }
}

void PayTableInterface::pageUp()
{
    if (_currentPage > 0) {
        --_currentPage;
        this->displayCurrentPage();
    }
}

void PayTableInterface::closeTable()
{
    emit tableCompleted();
}

void PayTableInterface::restoreConnections()
{
    connect(this, &PayTableInterface::softkeysForPage, _lcd, &GenericLCD::fillSoftkeys);
    connect(_input, &GenericInputHandler::softkeyPressed, this, &PayTableInterface::triggerSoftkey);
    connect(this, &PayTableInterface::resetDisplay, _lcd, &GenericLCD::setupPayTableDisplay);
    connect(this, &PayTableInterface::showTableRange, _lcd, &GenericLCD::displayTablePage);

    // Redisplay
    emit resetDisplay(_gameName);
    this->displayCurrentPage();
    this->softkeyPage();
}
