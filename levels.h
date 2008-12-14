/*
   Donkey Bolonkey
   Copyright (C) 2001 by David A. Capello

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

static LEVEL level1 =
{
  /* block */
  { LINE(    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(  S|R,  S|R,  S|R,  S|D,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(H|S|U,    0,    R,    R,    R,    R,    R,    R,    R,    D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,  B|R,  T|D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    D,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    0,    U,  E|L,  E|L,  E|L,  E|L,    L,    L,    L,    0,    0,    0,    0,    0,    0 ) },
  /* counter */
  16,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    FALSE, /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    FALSE, /* DONKEY_COLOR_LIGHTBLUE */
    FALSE, /* DONKEY_COLOR_BLUE */
    FALSE, /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 400, 600, 20, 5 },
  /* bg */
  BMP_BG1
};



static LEVEL level2 =
{
  /* block */
  { LINE(H|S|D,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(  S|D,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(  S|R,  S|R,    R,    R,    R,    R,    R,  T|R,    R,    R,    R,    R,    R,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,  B|U,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,  B|R,  T|D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,  E|L,  E|L,  E|L,  E|L,    L,    L,    L,    L,    L,    L,    L,    0,    0 ) },
  /* counter */
  20,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    TRUE,  /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    FALSE, /* DONKEY_COLOR_LIGHTBLUE */
    FALSE, /* DONKEY_COLOR_BLUE */
    FALSE, /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 350, 550, 18, 5 },
  /* bg */
  BMP_BG1
};



static LEVEL level3 =
{
  /* block */
  { LINE(    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  S|D,  S|L,H|S|L ),
    LINE(    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  S|D,    0,    0 ),
    LINE(    0,    0,    0,    R,    R,    R,    R,    R,    R,    R,    R,    R,    R,    D,    0,    0 ),
    LINE(    0,    0,    R,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    D,    L,  T|L,    L,    0,    0 ),
    LINE(    0,    0,  T|U,  B|L,    0,    0,    0,    0,    0,    0,    D,    0,  B|U,    0,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    R,    R,    R,    D,    0,    0 ),
    LINE(    0,    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,    0 ),
    LINE(    0,    0,    U,  E|L,  E|L,  E|L,  E|L,  E|L,    L,    L,    L,    L,    L,    L,    0,    0 ) },
  /* counter */
  28,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    TRUE,  /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    FALSE, /* DONKEY_COLOR_LIGHTBLUE */
    TRUE,  /* DONKEY_COLOR_BLUE */
    FALSE, /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 300, 500, 15, 4 },
  /* bg */
  BMP_BG2
};



static LEVEL level4 =
{
  /* block */
  { LINE(    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    R,    R,    R,    D,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 ),
    LINE(    0,    U,    0,    0,    R,    R,    D,    0,    0,    0,    R,    R,  T|R,    R,    D,    0 ),
    LINE(    0,    U,    0,    0,    0,    0,    D,    0,    0,    0,    U,    0,  B|U,    0,    D,    0 ),
    LINE(    0,    U,    L,    L,    L,    0,    D,    0,  B|D,    0,    U,    0,    0,    0,    D,    0 ),
    LINE(    0,    0,    0,    0,    U,    0,    R,    R,  T|R,    R,    U,    0,    0,    D,    L,  S|L ),
    LINE(    0,    R,    R,    R,    U,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,  S|U ),
    LINE(    0,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D,    0,  S|U ),
    LINE(    0,    U,  E|L,  E|L,  E|L,  E|L,  E|L,    L,    L,    L,    L,    L,    L,    L,    0,H|S|U ) },
  /* counter */
  36,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    TRUE,  /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    TRUE,  /* DONKEY_COLOR_LIGHTBLUE */
    TRUE,  /* DONKEY_COLOR_BLUE */
    FALSE, /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 250, 450, 10, 4 },
  /* bg */
  BMP_BG2
};



static LEVEL level5 =
{
  /* block */
  { LINE(    0,    R,    R,    R,    R,    D,    0,    0,H|S|D,    0,    R,    R,    R,    R,    R,    D ),
    LINE(    0,    U,    0,    0,    0,    D,    0,    0,  S|D,    0,    U,    0,    0,    0,    0,    D ),
    LINE(    0,    U,    0,    0,    0,    D,    0,    0,  S|D,    0,    U,    0,    0,    0,    0,    D ),
    LINE(    0,    U,    0,    0,    0,    R,    R,    R,    R,    R,    U,    0,    0,    0,    0,    D ),
    LINE(    0,  T|U,  B|L,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  B|R,  T|D ),
    LINE(    0,    U,    0,    0,    0,    0,    D,    L,  T|L,    L,    L,    0,    0,    0,    0,    D ),
    LINE(    0,    U,    0,    0,    0,    0,    D,    0,  B|U,    0,    U,    0,    0,    0,    0,    D ),
    LINE(    0,    U,    0,    0,    0,    0,    D,    0,    0,    0,    U,    0,    0,    0,    0,    D ),
    LINE(    0,  E|U,  E|L,  E|L,  E|L,  E|L,  E|L,    0,    0,    0,    U,    L,    L,    L,    L,    L ) },
  /* counter */
  44,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    TRUE,  /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    TRUE,  /* DONKEY_COLOR_LIGHTBLUE */
    TRUE,  /* DONKEY_COLOR_BLUE */
    TRUE,  /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 200, 400,  7, 4 },
  /* bg */
  BMP_BG3
};


static LEVEL level6 =
{
  /* block */
  { LINE(    R,    D,    0,    0,    0,    0,    0,    0,    0,    R,    R,    R,    D,    0,    R,    D ),
    LINE(    U,    R,    D,    0,    0,    0,    0,    0,    R,    U,    0,    0,    D,    0,    U,    D ),
    LINE(    U,    0,    R,    D,    0,    0,    0,    R,    U,    0,    0,    0,    D,    0,    U,    D ),
    LINE(    U,    0,    0,    R,    D,    0,    R,    U,    0,    0,    D,    L,    L,    0,    U,    D ),
    LINE(    U,    L,    0,    0,    R,  T|R,    U,    0,    0,  B|R,  T|D,    0,    0,    0,    U,    D ),
    LINE(    0,    U,    0,    0,    0,  B|U,    0,    0,    0,    0,    R,    R,    R,    R,    U,    D ),
    LINE(  S|R,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    D ),
    LINE(  S|U,    U,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  B|D,    D ),
    LINE(H|S|U,    U,  E|L,  E|L,  E|L,  E|L,  E|L,   E|L,   L,    L,    L,    L,    L,    L,  T|L,    L ) },
  /* counter */
  50,
  /* color_table */
  {
    FALSE, /* DONKEY_COLOR_WHITE */
    TRUE,  /* DONKEY_COLOR_RED */
    TRUE,  /* DONKEY_COLOR_ORANGE */
    TRUE,  /* DONKEY_COLOR_YELLOW */
    TRUE,  /* DONKEY_COLOR_GREEN */
    TRUE,  /* DONKEY_COLOR_LIGHTBLUE */
    TRUE,  /* DONKEY_COLOR_BLUE */
    TRUE,  /* DONKEY_COLOR_MAGENT */
  },
  /* donkey { move_freq_min, move_freq_max, move_freq_countdown, add_freq } */
  { 150, 350,  5, 4 },
  /* bg */
  BMP_BG3
};



