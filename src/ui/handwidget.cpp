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

const QString HandWidget::cardBackStyle  = "background-color: rgb(0,66,188);color: rgb(227,227,227);font-size: 20pt;";
const QString HandWidget::cardFrontStyle = "background-color: rgb(227,227,227);font-size: 48pt;";


HandWidget::HandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HandWidget)
{
    ui->setupUi(this);

    // There are 5 cards, selecting their respective hold buttons should indicate to the game orchestrator they are
    // to be preserved and not re-drawn
    connect(ui->holdBtnCard1, &QCheckBox::toggled,
            this, &HandWidget::card1Hold);
    connect(ui->holdBtnCard2, &QCheckBox::toggled,
            this, &HandWidget::card2Hold);
    connect(ui->holdBtnCard3, &QCheckBox::toggled,
            this, &HandWidget::card3Hold);
    connect(ui->holdBtnCard4, &QCheckBox::toggled,
            this, &HandWidget::card4Hold);
    connect(ui->holdBtnCard5, &QCheckBox::toggled,
            this, &HandWidget::card5Hold);
}

HandWidget::~HandWidget()
{
    delete ui;
}

void HandWidget::winningTextAndAmount(const QString &handString, quint32 winning)
{
    if (handString.isEmpty()) {
        ui->resultLabel->setText("");    // Nothing, no winning combination
    } else if (winning == 0) {
        ui->resultLabel->setText(handString);
    } else {
        ui->resultLabel->setText(handString + " - Win +" + QString::number(winning));
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
        ui->card1->setStyleSheet(cardFrontStyle + colorStyleSheet);
        break;
    case 1:
        ui->card2->setText(valueSymbol + "\n" + suitSymbol);
        ui->card2->setStyleSheet(cardFrontStyle + colorStyleSheet);
        break;
    case 2:
        ui->card3->setText(valueSymbol + "\n" + suitSymbol);
        ui->card3->setStyleSheet(cardFrontStyle + colorStyleSheet);
        break;
    case 3:
        ui->card4->setText(valueSymbol + "\n" + suitSymbol);
        ui->card4->setStyleSheet(cardFrontStyle + colorStyleSheet);
        break;
    case 4:
        ui->card5->setText(valueSymbol + "\n" + suitSymbol);
        ui->card5->setStyleSheet(cardFrontStyle + colorStyleSheet);
        break;
    default:
        // Should not happen!
        break;
    }
}

void HandWidget::showCardBacks(bool card1, bool card2, bool card3, bool card4, bool card5)
{
    if (card1) {
        ui->card1->setText("Chad's\nCasino");
        ui->card1->setStyleSheet(cardBackStyle);
    }
    if (card2) {
        ui->card2->setText("Chad's\nCasino");
        ui->card2->setStyleSheet(cardBackStyle);
    }
    if (card3) {
        ui->card3->setText("Chad's\nCasino");
        ui->card3->setStyleSheet(cardBackStyle);
    }
    if (card4) {
        ui->card4->setText("Chad's\nCasino");
        ui->card4->setStyleSheet(cardBackStyle);
    }
    if (card5) {
        ui->card5->setText("Chad's\nCasino");
        ui->card5->setStyleSheet(cardBackStyle);
    }
}

void HandWidget::resetAll()
{
    // Unset holds
    ui->holdBtnCard1->setCheckState(Qt::Unchecked);
    ui->holdBtnCard2->setCheckState(Qt::Unchecked);
    ui->holdBtnCard3->setCheckState(Qt::Unchecked);
    ui->holdBtnCard4->setCheckState(Qt::Unchecked);
    ui->holdBtnCard5->setCheckState(Qt::Unchecked);

    // Flip cards back
    ui->card1->setText("Chad's\nCasino");
    ui->card1->setStyleSheet(cardBackStyle);
    ui->card2->setText("Chad's\nCasino");
    ui->card2->setStyleSheet(cardBackStyle);
    ui->card3->setText("Chad's\nCasino");
    ui->card3->setStyleSheet(cardBackStyle);
    ui->card4->setText("Chad's\nCasino");
    ui->card4->setStyleSheet(cardBackStyle);
    ui->card5->setText("Chad's\nCasino");
    ui->card5->setStyleSheet(cardBackStyle);
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
