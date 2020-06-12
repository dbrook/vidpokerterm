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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>

/**
 * @brief The Account class handles adding/withdrawing/checking the balance of credits available for betting
 */
class Account : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Sets up an empty account
     */
    explicit Account(QObject *parent = nullptr);

    /**
     * @brief balance retrieves the number of credits available to bet
     *
     * @return number of credits in the account
     */
    quint32 balance() const;

    /**

     * @brief withdraw will try and pull credits out of the account to use for a bet
     *
     * @param[in]  amount     Number of credits to take out of the account
     *
     * @return true if the balance is sufficient to cover the withdrawl, false otherwise
     */
    bool withdraw(quint32 amount);

signals:

    // Emitted whenever the balance changes
    void balanceChanged(quint32 updatedBalance);

public slots:
    /**
     * @brief setBalance forces a new value to be held by the account
     *
     * @param[in]  newBal     Exact number of credits to save into the account
     */
    void setBalance(quint32 newBal);

    /**
     * @brief add
     *
     * @param[in]  amount     Number of credits to add to the account
     */
    void add(quint32 amount);

private:
    quint32 _balance;
};

#endif // ACCOUNT_H
