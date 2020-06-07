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

#ifndef HANDWIDGET_H
#define HANDWIDGET_H

#include <QWidget>

#include "playingcard.h"

namespace Ui {
class HandWidget;
}

class HandWidget : public QWidget
{
    Q_OBJECT

    const static QString cardBackStyle;
    const static QString cardFrontStyle;

public:
    explicit HandWidget(QWidget *parent = nullptr);
    ~HandWidget();

public slots:
    // For any winning hand (non-empty handString), the amount won and the analysis of the hand is shown
    void winningTextAndAmount(const QString &handString, quint32 winning);

    // Update a drawn card in its relevant position
    void revealCard(int cardIdx, PlayingCard card);

    // Hide cards (flip them back over) that are about to be redrawn
    void showCardBacks(bool card1, bool card2, bool card3, bool card4, bool card5);

    // Flip all cards back around and reset the hold checkboxes
    void resetAll();

    // Set the checkboxes so they may not be clicked when not in an active game
    void enableHolds(bool enableCheckBoxes);

signals:
    void card1Hold(bool cardIsHeld);
    void card2Hold(bool cardIsHeld);
    void card3Hold(bool cardIsHeld);
    void card4Hold(bool cardIsHeld);
    void card5Hold(bool cardIsHeld);

private:
    // Actual Qt widgets
    Ui::HandWidget *ui;
};

#endif // HANDWIDGET_H
