/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#include <allegro.h>
#include "dkbk.h"



#define L BLOCK_FLAG_LEFT
#define U BLOCK_FLAG_UP
#define R BLOCK_FLAG_RIGHT
#define D BLOCK_FLAG_DOWN
#define H BLOCK_FLAG_HOME
#define E BLOCK_FLAG_EXIT
#define S BLOCK_FLAG_STOP
#define B BLOCK_FLAG_BUBBLE
#define T BLOCK_FLAG_TRAP

#define LINE(b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16) \
  {{b1,0},{ b2,0},{ b3,0},{ b4,0},{ b5,0},{ b6,0},{ b7,0},{ b8,0}, \
   {b9,0},{b10,0},{b11,0},{b12,0},{b13,0},{b14,0},{b15,0},{b16,0}}



#include "levels.h"



static LEVEL *level = NULL;

static int doors_time = -1;
static int doors = 0;



static void get_funnel_pos()
{
  int x, y;

  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++)
      if (get_block_flags(x, y) & BLOCK_FLAG_EXIT) {
        funnel.x2 = x*BLOCK_WIDTH+BLOCK_WIDTH;
        funnel.y2 = y*BLOCK_HEIGHT+BLOCK_HEIGHT;
        if (funnel.x1 < 0) {
          funnel.x1 = funnel.x2-BLOCK_WIDTH;
          funnel.y1 = funnel.y2;
        }
      }
}



void init_level()
{
  reset_level(1);
}



void shutdown_level()
{
}



void update_level()
{
  if (doors_time >= 0) {
    doors_time++;
    if (doors_time > FRAMES_PER_SECOND)
      doors_time = -1;
  }
}



void draw_level(BITMAP *bmp)
{
  int doors_count;
  int x, y, flags;
  int xget, yget;

  /* background */
  blit(get_bitmap(level->bg), bmp, 0, 0, 0, 0, GAME_W, GAME_H);

  /* blocks */
  doors_count = 0;
  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++) {
      flags = get_block_flags(x, y);

      xget = -1;
      yget = -1;

      if (!(flags & BLOCK_FLAG_BUBBLE)) {
        if (flags & BLOCK_FLAG_EXIT) {
          if (doors_time < 0)
            xget = 0;
          else {
            if (doors_count >= doors)
              xget = 0;
            else if (doors_time < FRAMES_PER_SECOND/2)
              xget = 2 * doors_time / (FRAMES_PER_SECOND/2);
            else
              xget = 2 - 2 * (doors_time-FRAMES_PER_SECOND/2) / (FRAMES_PER_SECOND/4);

            xget = MID(0, xget, 2);
          }
          yget = 1;
          doors_count++;
        }
        else if (flags != 0) {
          xget = (flags & BLOCK_FLAG_TRAP)? 1: 0;
          yget = 0;
        }
      }

      if ((xget >= 0) && (yget >= 0)) {
        masked_blit(get_bitmap(BMP_BLOCKS), bmp,
          xget*BLOCK_WIDTH, yget*BLOCK_HEIGHT,
             x*BLOCK_WIDTH,    y*BLOCK_HEIGHT,
               BLOCK_WIDTH,      BLOCK_HEIGHT+yget*BLOCK_HEIGHT*2);
      }
    }

  /* funnel */
  triangle(bmp, funnel.x1, funnel.y1+BLOCK_HEIGHT,
                crusher.x, crusher.y,
                funnel.x1, funnel.y1+BLOCK_HEIGHT*3/2, makecol(128, 128, 0));

  triangle(bmp, funnel.x2, funnel.y2+BLOCK_HEIGHT,
                crusher.x+crusher.w, crusher.y,
                funnel.x2, funnel.y2+BLOCK_HEIGHT*3/2, makecol(128, 128, 0));
}



int reset_level(int level_num)
{
  int x, y;

  switch (level_num) {
    case 1: level = &level1; break;
    case 2: level = &level2; break;
    case 3: level = &level3; break;
    case 4: level = &level4; break;
    case 5: level = &level5; break;
    case 6: level = &level6; break;
    default:
      return FALSE;
  }

  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++)
      set_block_donkey(x, y, NULL);

  funnel.x1 = funnel.y1 = -1;
  funnel.x2 = funnel.y2 = -1;
  get_funnel_pos();

  shutdown_donkeys();
  init_donkeys();
  return TRUE;
}



LEVEL *get_level()
{
  return level;
}



int get_block_flags(int x, int y)
{
  return level->block[MID(0, y, LEVEL_HEIGHT-1)]
		     [MID(0, x, LEVEL_WIDTH-1)].flags;
}



DONKEY *get_block_donkey(int x, int y)
{
  return level->block[MID(0, y, LEVEL_HEIGHT-1)]
		     [MID(0, x,  LEVEL_WIDTH-1)].donkey;
}



DONKEY *set_block_donkey(int x, int y, DONKEY *donkey)
{
  DONKEY *old_donkey = get_block_donkey(x, y);

  level->block[MID(0, y, LEVEL_HEIGHT-1)]
	      [MID(0, x,  LEVEL_WIDTH-1)].donkey = donkey;

  return old_donkey;
}



void get_level_home(int *xout, int *yout)
{
  int x, y;

  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++)
      if (get_block_flags(x, y) & BLOCK_FLAG_HOME) {
        *xout = x;
        *yout = y;
        return;
      }
}



int get_level_bubble(int *xout, int *yout, int count)
{
  int x, y, c;

  c = 0;
  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++) {
      if (get_block_flags(x, y) & BLOCK_FLAG_BUBBLE) {
        if (count == c) {
          *xout = x;
          *yout = y;
          return TRUE;
        }
        c++;
      }
    }

  return FALSE;
}



int get_level_random_color()
{
  int c;
  do {
    c = rand()%DONKEY_COLOR_MAX;
  } while (!level->color_table[c]);
  return c;
}



void open_doors(int num)
{
  doors_time = 0;
  doors = num;
}
