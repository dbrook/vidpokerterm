# This file is NOT technically part of VidPokerTerm. This is to provide a link to
# the standard Linux SPI (serial peripheral interface) modules and the Raspberry Pi
# WiringPI interfaces such that the u8g2 functions may communicate to the LCD screen.
#
# Copy and fill in SOURCES and HEADERS with the files available from:
#   https://github.com/ribasco/u8g2-rpi-demo
#
# The actual u8g2 code should be retrieved and placed in the lcdinterface directory.

QT += core
TEMPLATE  = lib
CONFIG   += staticlib

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = lcdspi

DESTDIR = $$OUT_PWD/../bin

SOURCES += \
    spi.cpp \
    u8g2_hal_rpi.cpp

HEADERS += \
    spi.h \
    u8g2_hal_rpi.h
