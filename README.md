VidPokerTerm
============

VidPokerTerm is a simple Qt-based implementation of a video poker terminal.

Players can add credits to an account and deal/hold/draw cards in the typical
playing pattern of video poker.

As a proof-of-concept, the first implementation of video poker is based on
the venerable Jacks-or-Better game:

  https://en.wikipedia.org/wiki/Video_poker#Jacks_or_Better

Additional games (aside from the included Jacks-or-Better) can be created by
inheriting new classed from the base PokerGame class. More will be added as
we build the game.

Modes
-----
There are two ways to compile this game:
 - Full Qt GUI-based interface requiring a window manager
 - LCD-based interface designed for a 128x64 pixel ST7920 screen over SPI

See the vidpokerterm.pro file to select which environments to compile by
(un)commenting the appropriate line.

Building
--------
To successfully build from source for a regular Qt-based GUI, you will need:
 - C++ development libraries
 - Qt development framework

If you wish to use the ST7920 LCD interface, you will need:
 - Raspberry Pi (see RasPi_CFAG12864_WiringDiag.png for all connections)
 - Configure the Raspberry Pi to enable SPI
 - Crystalfontz CFAG12864 LCD display (be advised it has 5v logic levels!)
 - Logic level translator for Pi<-->LCD communication (Sparkfun BOB-12009)
 - wiringPi C library
 - Copy of the u8g2 C library, copied into ./lcdinterface/*
 - The display should be connected to SPI0 of the Raspberry Pi
