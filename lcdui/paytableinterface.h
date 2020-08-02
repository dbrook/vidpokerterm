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

#ifndef PAYTABLEINTERFACE_H
#define PAYTABLEINTERFACE_H

#include "lcdinterface.h"
#include "genericlcd.h"
#include "genericinputhandler.h"

#include "gameorchestrator.h"

#include <QObject>
#include <QVector>

class PayTableInterface : public LCDInterface
{
    Q_OBJECT
public:
    explicit PayTableInterface(int                  nbSoftkeys,
                               GenericLCD          *lcdScreen,
                               GenericInputHandler *inputs,
                               GameOrchestrator    *orchestrator,
                               const QString       &gameName,
                               QObject             *parent = nullptr);

    ~PayTableInterface();

public slots:
    /**
     * @brief displayCurrentPage
     */
    void displayCurrentPage();

    /**
     * @brief Moved down 1 page and triggers a display of the relevant hands and payouts
     */
    void pageDown();

    /**
     * @brief Moves up 1 page and triggers a display of the relevant hands and payouts
     */
    void pageUp();

    /**
     * @brief Slot to trigger signal to previous interface that the table may be closed
     */
    void closeTable();

    /**
     * @brief Setup the connections for the interface
     */
    void restoreConnections();

signals:
    void resetDisplay(const QString &gameName);

    void tableCompleted();

    void showTableRange(QVector<QPair<const QString, int>> table, int startIdx, int nbItems);

private:
    GenericLCD          *_lcd;
    GenericInputHandler *_input;
    GameOrchestrator    *_synchroOrc;

    QString                            _gameName;
    int                                _currentPage;
    QVector<QPair<const QString, int>> _payTable;
};

#endif // PAYTABLEINTERFACE_H
