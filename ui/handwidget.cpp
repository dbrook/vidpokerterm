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

#include "handwidget.h"
#include "ui_handwidget.h"

HandWidget::HandWidget(bool           extraHand,
                       const QSize   &cardSize,
                       const QString &fontSize,
                       const QString &winFontSize,
                       QWidget       *parent) :
    QWidget(parent),
    _displayCardsOnly(extraHand),
    _cardSize(cardSize),
    _fontSize(fontSize),
    _winFontSize(winFontSize),
    ui(new Ui::HandWidget)
{
    ui->setupUi(this);

    // Apply styling to the cards
    _cardBackStyleSheet = "border-radius:10px;color:rgb(227,227,227);"
            "background-color: qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 0, 74, 255),"
            " stop:1 rgba(0, 0, 255, 255));";

    _cardFrontStyleSheet = "border-radius:10px;"
            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(185, 185, 185, 255),"
            "stop:1 rgba(255, 255, 255, 255));font-size:" + _fontSize + "pt;";

    if (!_winFontSize.isEmpty()) {
        ui->resultLabel->setStyleSheet("font-size:" + _winFontSize + "pt;");
    }

    // And make sure the size obeys what the UI requested
    ui->card1->setMinimumSize(_cardSize);
    ui->card2->setMinimumSize(_cardSize);
    ui->card3->setMinimumSize(_cardSize);
    ui->card4->setMinimumSize(_cardSize);
    ui->card5->setMinimumSize(_cardSize);

    // There are 5 cards, selecting their respective hold buttons should indicate to the game orchestrator they are
    // to be preserved and not re-drawn
    if (!_displayCardsOnly) {
        // Primary hand: show and allow use of the hold buttons
        connect(ui->holdBtnCard1, &QPushButton::toggled, this, &HandWidget::card1Hold);
        connect(ui->holdBtnCard2, &QPushButton::toggled, this, &HandWidget::card2Hold);
        connect(ui->holdBtnCard3, &QPushButton::toggled, this, &HandWidget::card3Hold);
        connect(ui->holdBtnCard4, &QPushButton::toggled, this, &HandWidget::card4Hold);
        connect(ui->holdBtnCard5, &QPushButton::toggled, this, &HandWidget::card5Hold);
    } else {
        // Extra hand(s): only for display, no hold button control
        ui->holdBtnCard1->deleteLater();
        ui->holdBtnCard2->deleteLater();
        ui->holdBtnCard3->deleteLater();
        ui->holdBtnCard4->deleteLater();
        ui->holdBtnCard5->deleteLater();
        ui->resultLabel->setText("");
    }
}

HandWidget::~HandWidget()
{
    delete ui;
}

void HandWidget::winningTextAndAmount(const QString &handString, quint32 winning)
{
    if (handString.isEmpty()) {
        ui->resultLabel->setText("");
    } else if (winning == 0) {
        ui->resultLabel->setText(handString);
    } else {
        ui->resultLabel->setText(handString + " +" + QString::number(winning));
    }
}

void HandWidget::revealCard(int cardIdx, PlayingCard card)
{
    PlayingCard::CardSuit suit = card.suit();
    PlayingCard::CardValue val = card.value();

    // Render the card as actual readable text and not the enum values they're based off of
    // TODO: this should be a static member function in PlayingCard??
    QString suitSymbol  = "";
    QString valueSymbol = "";
    QString colorStyleSheet = "";

    // Opted to use 4 different colors for the 4 suits. I saw a platform use this and I like it for identifying flushes
    switch (suit) {
    case PlayingCard::DIAMOND:
        suitSymbol = "♦";
        colorStyleSheet = "color: rgb(0,100,180);";   // Diamonds are blue
        break;
    case PlayingCard::CLUB:
        suitSymbol = "♣";
        colorStyleSheet = "color: rgb(0,0,0);";       // Keep Clubs black
        break;
    case PlayingCard::HEART:
        suitSymbol = "♥";
        colorStyleSheet = "color: rgb(255,0,0);";     // Keep Hearts red
        break;
    case PlayingCard::SPADE:
        suitSymbol = "♠";
        colorStyleSheet = "color: rgb(0,140,75);";   // Spades are green
        break;
    default:
        // should not happen!
        break;
    }

    switch (val) {
    case PlayingCard::TWO:
        valueSymbol = "2";
        break;
    case PlayingCard::THREE:
        valueSymbol = "3";
        break;
    case PlayingCard::FOUR:
        valueSymbol = "4";
        break;
    case PlayingCard::FIVE:
        valueSymbol = "5";
        break;
    case PlayingCard::SIX:
        valueSymbol = "6";
        break;
    case PlayingCard::SEVEN:
        valueSymbol = "7";
        break;
    case PlayingCard::EIGHT:
        valueSymbol = "8";
        break;
    case PlayingCard::NINE:
        valueSymbol = "9";
        break;
    case PlayingCard::TEN:
        valueSymbol = "10";
        break;
    case PlayingCard::JACK:
        valueSymbol = "J";
        break;
    case PlayingCard::QUEEN:
        valueSymbol = "Q";
        break;
    case PlayingCard::KING:
        valueSymbol = "K";
        break;
    case PlayingCard::ACE:
        valueSymbol = "A";
        break;
    default:
        // should not happen!
        break;
    }

    // Pick the right card to render, "flip it over" (turn it white), then render the card text
    switch (cardIdx) {
    case 0:
        ui->card1->setText(valueSymbol + "\n" + suitSymbol);
        ui->card1->setStyleSheet(_cardFrontStyleSheet + colorStyleSheet);
        break;
    case 1:
        ui->card2->setText(valueSymbol + "\n" + suitSymbol);
        ui->card2->setStyleSheet(_cardFrontStyleSheet + colorStyleSheet);
        break;
    case 2:
        ui->card3->setText(valueSymbol + "\n" + suitSymbol);
        ui->card3->setStyleSheet(_cardFrontStyleSheet + colorStyleSheet);
        break;
    case 3:
        ui->card4->setText(valueSymbol + "\n" + suitSymbol);
        ui->card4->setStyleSheet(_cardFrontStyleSheet + colorStyleSheet);
        break;
    case 4:
        ui->card5->setText(valueSymbol + "\n" + suitSymbol);
        ui->card5->setStyleSheet(_cardFrontStyleSheet + colorStyleSheet);
        break;
    default:
        // Should not happen!
        break;
    }
}

void HandWidget::showCardBacks(bool card1, bool card2, bool card3, bool card4, bool card5)
{
    if (card1) {
        ui->card1->setText("");
        ui->card1->setStyleSheet(_cardBackStyleSheet);
    }
    if (card2) {
        ui->card2->setText("");
        ui->card2->setStyleSheet(_cardBackStyleSheet);
    }
    if (card3) {
        ui->card3->setText("");
        ui->card3->setStyleSheet(_cardBackStyleSheet);
    }
    if (card4) {
        ui->card4->setText("");
        ui->card4->setStyleSheet(_cardBackStyleSheet);
    }
    if (card5) {
        ui->card5->setText("");
        ui->card5->setStyleSheet(_cardBackStyleSheet);
    }
}

void HandWidget::resetAll()
{
    // Unset holds
    if (!_displayCardsOnly) {
        ui->holdBtnCard1->setChecked(false);
        ui->holdBtnCard2->setChecked(false);
        ui->holdBtnCard3->setChecked(false);
        ui->holdBtnCard4->setChecked(false);
        ui->holdBtnCard5->setChecked(false);
    }

    // Flip cards back
    ui->card1->setText("");
    ui->card1->setStyleSheet(_cardBackStyleSheet);
    ui->card2->setText("");
    ui->card2->setStyleSheet(_cardBackStyleSheet);
    ui->card3->setText("");
    ui->card3->setStyleSheet(_cardBackStyleSheet);
    ui->card4->setText("");
    ui->card4->setStyleSheet(_cardBackStyleSheet);
    ui->card5->setText("");
    ui->card5->setStyleSheet(_cardBackStyleSheet);

    // Hide any winning string
    ui->resultLabel->setText("");
}

void HandWidget::enableHolds(bool enableCheckBoxes)
{
    if (!enableCheckBoxes) {
        ui->holdBtnCard1->setDisabled(true);
        ui->holdBtnCard2->setDisabled(true);
        ui->holdBtnCard3->setDisabled(true);
        ui->holdBtnCard4->setDisabled(true);
        ui->holdBtnCard5->setDisabled(true);
    } else {
        ui->holdBtnCard1->setDisabled(false);
        ui->holdBtnCard2->setDisabled(false);
        ui->holdBtnCard3->setDisabled(false);
        ui->holdBtnCard4->setDisabled(false);
        ui->holdBtnCard5->setDisabled(false);
    }
}
