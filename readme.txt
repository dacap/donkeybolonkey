Donkey Bolonkey
Copyright (C) 2001, 2008 por David Capello

This game doesn't have any guarantee, read file COPYING for more details
about this.

INTRODUCTION

  This games is my fastest creation: DEVELOPED IN JUST THREE DAYS!!!
  Without looking at external code or using others' graphics (except the
  backgrounds :-). Well, here it is, a simple (improved or worsened)
  Rat-Poker clon, only with donkeys... why?... well, it's a long story...

INSTALLATION

  If you have the source code, Allegro properly installed, the DJGPP
  compiler and the `make' tool, you shouldn't have problems to compile
  this program from inside its directory.

	C:\dkbk>make

  Also, now there's a Unix `makefile'. Use:

	~/dkbk$ make -f makefile.uni

  Theoretically it should work.

ABOUT THE SPEEDHACK

  This game was created for the "SpeedHack" 2001 competition, where
  several independent programmers meet and write games alone in just
  about three days (a weekend).

  The requeriments of the competition were:
  - a puzzle like game;
  - a high scores table;
  - you should have at least one donkey (the game is full of them ;-)
  - there should be a particle system (shortly after playing the
    game... you will know why I used it :-)
  - you should have a "banner" (BANNER VISION pops up here)
  - the source code and datafiles (compressed in a ZIP), should not
    be bigger than 250 KB;

  The competition was organized by Arron Shutt, and for more information
  you can visit:

    http://www.ashutt.demon.co.uk/speedhack/

  You will find info about this, and even the games created in the
  competition.

ABOUT THE GAME

  The goal of the game is to order donkeys of the same color. Thus,
  for example, when you join 3 or more red donkeys, they will "vanish"
  in a "weird" way... you will see. To order the donkeys you use the
  keys:

  TAB				   Change the current bubble.
  SPACEBAR			   Exchange the bubble's donkey with the
				   donkey in the block with a ruby in
				   front of it.

  Other extra keys you may use:

  F12				   Take a screenshot saving it as a
				   `capt????.pcx' file.
  P				   Pause, stops the game until you
				   press it again.
  ESC				   Quit the game quickly. Take care
				   when using it.

LEVELS

  If you want to add a level (you can do it only with the source
  code, Allegro and a supported compiler), you have to modify the
  files `levels.h' and `level.c'. You can also modify the existing
  levels if you prefer it and find it easier.

  You only have to take a look at the `levels.h' file and find out
  how it all works. The levels are composed by blocks, which have a
  specific letter (remember that all blocks have a direction, so
  you will have to use the OR operator 'I' to mix several types):

  L: Left	The block makes the donkeys go left.
  U: Up		The block makes the donkeys go up.
  R: Right	The block makes the donkeys go right.
  D: Down	The block makes the donkeys go down.
  H: Home	Donkeys start from here. You can have only one home
		per level, otherwise only the first home will be
		used (from the top left corner to the bottom right
		corner).
  E: Exit	This is an exit, each level has to have at least three
		exists, it's recommended to put them left at the bottom
		(near the triturator).
  S: Stop	These are blocks where the donkey can wait another one.
		They can be both in the home block or outside where
		they can enter the "round" (since the only donkeys which
		wait are those who enter).
  B: Bubble	Where you place the bubble, you also have to indicate it's
		direction with the OR operator, to know from which block
		you take donkeys.
  T: Trap	You can add this to a normal direction block to make it
		the tarket of some bubble (these are those blocks with
		a ruby which can be found in the game).

  You can find `#include "levels.h"' in the `level.c' file to find
  more information about this. It's also handy to read the `reset_leve'
  function of that file, since it's that one which uses a global
  `switch' to select the correct level, and don't worry, if you
  add more case statements (cas 7, case 8...) to add more levels,
  those will be called without problems.

CREDITS

  Programmer:
	David Capello
	Programming in C/C++ with Allegro, DJGPP and RHIDE.
	Total schedule:
	Friday 26: from 10am to 2am of the next day.
	Saturday 27: from 8am to 2am of the next day.
	Sunday 28: from 8am to 4am of the monday 29.
	The monday at 4:30 am I emailed the ZIP to the personal
	address of Arron.

	Total hours spent programming:
	16hs + 18hs + 20hs = 54 hours awaken IN TOTAL
	Which gives us that I slept only 18 hourse that
	weekend.
	You should also reduce those by a few hourse for
	eating and such things (ie: going to the bathroom).

  Graphics:
	David Capello
	All of them were makde with my program 2D SPRITEStudio,
	which still lacks Undo and Redo functions, but I made it.
	The "Donkey Bolonkey" logo and "Game Over" where
	done with 3D MAX, the three backgrounds come from a
	CD, and the fonts where made with TTF2PCX.

	Some ideas (models drawn on paper) to create the
	triturator and the scoreboard, come from my brother.

  Sounds:
	* Slarm: I don't know where that came from :-), I just
	happened to have it.

	* Bubble: done by my brother Gaspar.

	* Crusher: Those were hits on a table by my brother, and
	I did the chawsain sound with the mouth, both where mixed in.

	* Motor: the motor sound, believe it or not, cames from a
	Playstation Colin McRae Rally game, there's a part where some
	mechanics fix the car...

	* Donkey scream: We took about an hour and a half trying to do
	sounds of donkeys screaming, but all sounded weird and even
	made us feel pity for the donkeys, in the end, we decided to
	put a voice which said "Hey, you son of a ...", and I made it
	sound higher.

	* Starting voice: That voice you hear, it's mine, without
	deformations, if I sound horrible, excuse me...

  Music:
	Unluckily we didn't have time to put some music in the game,
	which I would have liked a lot.

  Testers:
	I personally didn't play it much, until I said my brothers to
	WARN ME WHEN I BECOME STUPID AND DON'T STOP PLAYING... since it
	happens that I loose time testing the game... or better said
	playing with it.
	The truth is that the one who most tested the game after the
	competition was my brother Gaspar, who reached the end
	without continuing with a score of 7200. Which means...
	...IT'S POSSIBLE TO REACH THE END OF THE GAME!!!... although it
	may be a little bit hard.

  Title Donkey Bolonkey:
	My brother Gaspar's idea.
	(if I didn't put this... he would hit me :-)

  Documentation:
	What you read now, was done outside of the SpeedHack
	competition, since I was very tired then to write the
	documentation.
	Thanks to Grzegorz Adam Hankiewicz to translate the
        Spanish files (leadme.txt and ChangeLog.es) to English!!!

  Thanks to:
	Specially:
	* DJ Delorie for the DJGPP compiler.
	* Charles W Sandmann for the DPMI MS-DOS server.
	* Robert Höhne for RHIDE.
	* Shawn Hargreaves for starting and creating the Allegro lib.
	* Arron Shutt for organising the best game programming
	  contest and standing us all.

LINKS

  If you want to see more things, here are some interesting links:

  DJGPP:	http://www.delorie.com/djgpp/
  RHIDE:	http://www.lanet.lv/~pavenis/rhide.html
  Allegro:	http://www.talula.demon.co.uk/allegro/
  SpeedHack:	http://www.ashutt.demon.co.uk/speedhack/

EXTRA

  Finally, I added some old images of the game without being completed.

  --
  Copyright (C) 2001 David Capello

  davidcapello@gmail.com
  http://davidcapello.com/games/donkey-bolonkey/
