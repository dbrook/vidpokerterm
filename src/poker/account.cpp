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

#include "account.h"

Account::Account() : _balance(0)
{

}

quint32 Account::balance()
{
    return _balance;
}

bool Account::withdraw(quint32 amount)
{
    if (amount > _balance) {
        return false;
    }
    _balance -= amount;
    return true;
}

void Account::add(quint32 amount)
{
    _balance += amount;
}
