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

Building
--------
To successfully build from source, you will need:
 - C++ development libraries
 - Qt development framework
(the requirements are pretty simple as there is not much to this kind of game)
