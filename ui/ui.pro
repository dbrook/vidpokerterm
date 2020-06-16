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

# Here we are building the main GUI application by linking in (statically) the
# core poker game logic handler object and building the UI wrappers around it.

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = vidpokerterm
TEMPLATE = app

DESTDIR = $$OUT_PWD/../bin

LIBS *= -L$$DESTDIR -lpokerbe

INCLUDEPATH += $$PWD \
    $$PWD/../poker

SOURCES += \
    $$PWD/gameorchestratorwindow.cpp \
    $$PWD/handwidget.cpp \
    $$PWD/main.cpp \
    $$PWD/gameaccountwindow.cpp

HEADERS += \
    $$PWD/gameaccountwindow.h \
    $$PWD/gameorchestratorwindow.h \
    $$PWD/handwidget.h

FORMS += \
    $$PWD/gameaccountwindow.ui \
    $$PWD/gameorchestratorwindow.ui \
    $$PWD/handwidget.ui
