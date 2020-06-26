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

#ifndef LCDINTERFACE_H
#define LCDINTERFACE_H

#include <QObject>
#include <QVector>

/**
 * @brief The LCDInterface class is a generic user interface skeleton. It provides a generic interface for handling
 *        softkey setup and interaction.
 */
class LCDInterface : public QObject
{
    Q_OBJECT
public:
    explicit LCDInterface(int nbSoftkeys, QObject *parent = nullptr);

    virtual ~LCDInterface();

    /**
     * @brief addSoftkeyFunction adds a label and function to the list of softkeys
     */
    void addSoftkeyFunction(const QString &functionName, void (LCDInterface::*)());

    /**
     * @brief finishSoftkeys must be called when no more softkeys are added, this function will add a final return key
     */
    void finishSoftkeys();

public slots:
    /**
     * @brief softkeys emits all the softkeys that can fit on the current page. If there are multiple pages, then the
     *        last key will always be a "next" button
     */
    void softkeys();

    /**
     * @brief nextSoftkeyPage gets the next page of softkeys (if available)
     */
    void nextSoftkeyPage();

    /**
     * @brief noopSoftkeyItem is a function that will do nothing (in case "padding" of keys is needed)
     */
    void noopSoftkeyItem();

    /**
     * @brief triggerSoftkey calls the function pointed to by the softkey
     *
     * @param displayedKeyIdx     Index of the displayed screen softkey (not the overall QVector's index)
     */
    void triggerSoftkey(int displayedKeyIdx);

signals:
    void softkeyAtPosition(int keyPosition, const QString &label);

protected:
    QVector<QPair<QString, void (LCDInterface::*)()>> _softkeys;
    int                                               _nbSoftkeys;
    int                                               _softkeyPage;
};

#endif // LCDINTERFACE_H
