# VidPokerTerm
# Copyright (c) 2020 Daniel Brook (danb358 {at} gmail {dot} com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDEPATH += $$PWD

DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/commonhandanalysis.h \
    $$PWD/deck.h \
    $$PWD/hand.h \
    $$PWD/jacksorbetter.h \
    $$PWD/playingcard.h \
    $$PWD/pokergame.h

SOURCES += \
    $$PWD/commonhandanalysis.cpp \
    $$PWD/deck.cpp \
    $$PWD/hand.cpp \
    $$PWD/jacksorbetter.cpp \
    $$PWD/playingcard.cpp \
    $$PWD/pokergame.cpp
