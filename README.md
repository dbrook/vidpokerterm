VidPokerTerm
============
VidPokerTerm is a simple Qt-based implementation of a video poker terminal.

Players can add credits to an account and deal/hold/draw cards in the typical
playing pattern of video poker.

As a proof-of-concept, the first implementation of video poker is based on
the venerable Jacks-or-Better game:

  https://en.wikipedia.org/wiki/Video_poker#Jacks_or_Better

Additional games (aside from the included Jacks-or-Better) can be created by
inheriting new classes from the base PokerGame class. More will be added as
we build the game. You will find an instance of "Bonus Poker" too, but there
is no pay table associated with it (just there to test the UI feature), so
you will only ever lose money with it ... a good lesson for gambling software ;-)

Modes
-----
There are two ways to compile this game:
 - Full Qt GUI-based interface requiring a window manager
 - LCD-based interface designed for a 128x64 pixel ST7920 screen over SPI

See the vidpokerterm.pro file to select which environments to compile by
(un)commenting the appropriate lines.

Building
--------
Regardless of the interface used, you will need the following installed (note
that these are Debian-based package names, your distribution may vary, this
should also bring in any build dependencies, but if not you'll need a compiler too):
 - sudo apt install qt5-qmake qt5-default

If you're planning on using the Raspberry Pi with the ST7920 interface, also run:
 - sudo apt install wiringpi

The whole suite (see comments in vidpokerterm.pro) may be built by simply:
 - cd vidpokerterm
 - qmake
 - make
 - ./bin/vidpokerterm (starts the GUI, be advised it is in a very rough state)
 - ./bin/lcdpokerterm (starts the LCD, using -k enables keyboard GPIO press emulation mode)

The ST7920 LCD on a Raspberry Pi requires:
 - a Raspberry Pi (see RasPi_CFAG12864_WiringDiag.png for all connections)
 - enabling the SPI interface on said Raspberry Pi (use raspi-config, make the change, then reboot)
 - a copy of the u8g2 C library (see instructions in lcdinterface/lcdinterface.pro
 - a Crystalfontz CFAG12864 LCD display (be advised it has 5v logic levels!)
 - a logic level translator for Pi<-->LCD communication (Sparkfun BOB-12009)
 - connecting the display to SPI0 of the Raspberry Pi
 - 10 momentary switches
 - See the RasPi_CFAG12864_WiringDiag.odg/png file for a connection guide
   (you can also find instructions on making a bracket at http://www.danbrook.org/vidpokerterm.html)
 - TODO: make a 3d-printable case to house everything (this is very much a work-in-progress)
