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

#include "lcdinterface.h"

#include <QDebug>

LCDInterface::LCDInterface(int nbSoftkeys, QObject *parent)
    : QObject      (parent),
      _nbSoftkeys  (nbSoftkeys),
      _softkeyPage (0)
{

}

LCDInterface::~LCDInterface() {}

void LCDInterface::addSoftkeyFunction(const QString &functionName, void (LCDInterface::*function)())
{
    // Add the new function and label
    QPair<QString, void (LCDInterface::*)()> functionAndFuncPtr(functionName, function);
    _softkeys.push_back(functionAndFuncPtr);
}

void LCDInterface::finishSoftkeys()
{
    // Put in "next" buttons in case of overflow
    QPair<QString, void (LCDInterface::*)()>          nextButton("   ->", &LCDInterface::nextSoftkeyPage);
    QPair<QString, void (LCDInterface::*)()>          noopButton("", &LCDInterface::noopSoftkeyItem);
    QVector<QPair<QString, void (LCDInterface::*)()>> paddedSoftkeys;

    if (_softkeys.size() > _nbSoftkeys) {
        int nextBtnOffset = _nbSoftkeys;
        int revisedButtonPosition = 1;

        for (QPair<QString, void (LCDInterface::*)()> softkeyItem : _softkeys) {
            paddedSoftkeys.push_back(softkeyItem);
            ++revisedButtonPosition;
            if (revisedButtonPosition % nextBtnOffset == 0) {
                paddedSoftkeys.push_back(nextButton);
                ++revisedButtonPosition;
            }
        };

        // Put a final button (to be able to go back to the beginning) after putting any padding
        if (revisedButtonPosition % _nbSoftkeys != 1) {
            while (revisedButtonPosition % _nbSoftkeys != 0) {
                paddedSoftkeys.push_back(noopButton);
                ++revisedButtonPosition;
            };
            paddedSoftkeys.push_back(nextButton);
        }

        // Overwrite the old key vector with the 'padded' one
        _softkeys = paddedSoftkeys;
    } else {
        // Put no-op buttons to fill the rest of the key position (so as not to get a seg fault)
        for (int emptyKeyIdx = _softkeys.size(); emptyKeyIdx < _nbSoftkeys; ++emptyKeyIdx) {
            _softkeys.push_back(noopButton);
        }
    }

//    for (QPair<QString, void (LCDInterface::*)()> button : _softkeys) {
//        qDebug() << "Button: " << button.first;
//    }
}

void LCDInterface::softkeys()
{
    int keysDrawn = 0;
    for (int startKey = _softkeyPage * _nbSoftkeys;
         startKey < _softkeys.size() && keysDrawn < _nbSoftkeys;
         ++startKey, ++keysDrawn) {
        emit softkeyAtPosition(keysDrawn, _softkeys[startKey].first);
    }

    // If we didn't use up all the positions, make sure to zero out the other keys with a bunch of spaces
    for (; keysDrawn < _nbSoftkeys; ++keysDrawn) {
        emit softkeyAtPosition(keysDrawn, "              ");
    }
}

void LCDInterface::nextSoftkeyPage()
{
    // Choose the next "page"
    _softkeyPage++;
//    qDebug() << "Next page: " << _softkeyPage
//             << "_softkeyPage * _nbSoftkeys == " << _softkeyPage * _nbSoftkeys
//             << "_softkeys.size()" << _softkeys.size();

    // Did we run out of keys? Reset to the first page
    if (_softkeyPage * _nbSoftkeys >= _softkeys.size()) {
        _softkeyPage = 0;
    }

    // Then re-emit the keys
    this->softkeys();
}

void LCDInterface::noopSoftkeyItem()
{
    ;
}

void LCDInterface::triggerSoftkey(int displayedKeyIdx)
{
//    qDebug() << "Trying to activate key: " << _softkeyPage * _nbSoftkeys + displayedKeyIdx;
    (this->*_softkeys[_softkeyPage * _nbSoftkeys + displayedKeyIdx].second)();
}
