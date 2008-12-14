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

#include <allegro.h>
#include "dkbk.h"



static int start = FALSE;

static int title_time;
static int title_sound;

static int hiscore_state = FALSE;



static void init_title()
{
  clear(screen);
  set_palette_range(datafile[PAL_TITLE].dat, 0x90, 0xff, TRUE);

  title_time = 0;
  title_sound = FALSE;
}



static void shutdown_title()
{
}



static int update_title()
{
  if (key[KEY_ESC]) {
    start = FALSE;
    return TRUE;
  }
  else if (key[KEY_ENTER]) {
    start = TRUE;
    return TRUE;
  }

  title_time++;
  if ((!title_sound) && (title_time > FRAMES_PER_SECOND/2)) {
    title_sound = TRUE;
    play_sample(datafile[SAMP_TITLE].dat, 255, 128, 1000, FALSE);
  }
  
  return FALSE;
}



static void draw_title(BITMAP *bmp)
{
  int f;

  /* title */
  if (title_time > (FRAMES_PER_SECOND/2)) {
    blit(get_bitmap(BMP_TITLE), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
  }
  else {
    f = 100 * title_time / (FRAMES_PER_SECOND/2);

    stretch_blit(get_bitmap(BMP_TITLE), bmp,
      0, 0, get_bitmap(BMP_TITLE)->w, get_bitmap(BMP_TITLE)->h,
      SCREEN_W-SCREEN_W*f/100,
      -SCREEN_H+SCREEN_H*f/100,
      SCREEN_W+(SCREEN_W*2-SCREEN_W*2*f/100),
      SCREEN_H+(SCREEN_H*2-SCREEN_H*2*f/100));
  }

  /* background */
  if (title_time > FRAMES_PER_SECOND) {
    if (title_time > FRAMES_PER_SECOND*3/2) {
      if (title_time < FRAMES_PER_SECOND*2)
        masked_blit(get_bitmap(BMP_TITLE_BG), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      else if (title_time < FRAMES_PER_SECOND*3)
        masked_blit(get_bitmap(BMP_TITLE_BG2), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      else if (title_time < FRAMES_PER_SECOND*4)
        masked_blit(get_bitmap(BMP_TITLE_BG), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      else if (title_time < FRAMES_PER_SECOND*9/2) {
        f = 100 * (title_time-FRAMES_PER_SECOND*4) / (FRAMES_PER_SECOND/2);
        masked_blit(get_bitmap(BMP_TITLE_BG), bmp,
          0, 0, 0, SCREEN_H*f/100, SCREEN_W, SCREEN_H);
      }
    }
    else {
      f = 100 * (title_time-FRAMES_PER_SECOND) / (FRAMES_PER_SECOND/2);
      masked_blit(get_bitmap(BMP_TITLE_BG), bmp,
        0, 0, 0, SCREEN_H-SCREEN_H*f/100, SCREEN_W, SCREEN_H);
    }
  }

  /* message */
  if (title_time > FRAMES_PER_SECOND) {
    if (title_time%FRAMES_PER_SECOND < FRAMES_PER_SECOND/2) {
      text_mode(-1);
      textout_centre(bmp, datafile[FONT_GAME].dat,
        "Press ENTER to start", SCREEN_W/2, SCREEN_H-64, -1);
      textout_centre(bmp, datafile[FONT_GAME].dat,
        "Press ESC to exit", SCREEN_W/2, SCREEN_H-40, -1);
    }
  }
}



static int count;

static void inc_count()
{
  count++;
}

END_OF_STATIC_FUNCTION(inc_count);



int title_screen()
{
  BITMAP *bmp = create_bitmap(SCREEN_W, SCREEN_H);
  int gameover = FALSE;

  init_title();

  LOCK_VARIABLE(count);
  LOCK_FUNCTION(inc_count);

  count = 0;
  install_int_ex(inc_count, BPS_TO_TIMER(FRAMES_PER_SECOND));

  my_clear_keybuf();

  while (!gameover) {
    while ((count > 0) && (!gameover)) {
      poll_keyboard();

      if (!hiscore_state)
        gameover = update_title();
      else
        hiscore_state = !update_hiscore();

      count--;
    }

    clear(bmp);

    if (!hiscore_state)
      draw_title(bmp);
    else
      draw_hiscore(bmp);

    vsync();
    blit(bmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    /* create new capture */
    if (key[KEY_F12])
      create_capture(bmp);
  }

  remove_int(inc_count);

  shutdown_title();

  destroy_bitmap(bmp);
  return start;
}



void active_hiscore()
{
  hiscore_state = TRUE;
}



void draw_warning(BITMAP *bmp)
{
  static char *warning_text[] = {
     "",
     "WARNING",
     "",
     "",
     "This game has violence",
     "scenes, please, if you",
     "love donkeys... don't",
     "play it...",
     "",
     "Don't say later",
     "I didn't warn you...",
     "",
     "",
     "Press any key to continue",
     NULL,
  };
  int c;

  text_mode(-1);

  for (c=0; warning_text[c]; c++)
    textout_centre(bmp, datafile[FONT_GAME].dat, warning_text[c],
      SCREEN_W/2, text_height(datafile[FONT_GAME].dat)*c, -1);

  my_clear_keybuf();
  readkey();
  my_clear_keybuf();

  clear(bmp);
}



void draw_controls(BITMAP *bmp)
{
  blit(get_bitmap(BMP_BG1), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  textout_centre(bmp, datafile[FONT_GAME].dat, "CONTROLS", SCREEN_W/2, 16, -1);
  textout(bmp, datafile[FONT_GAME].dat, "SPACE BAR - traps a donkey", 8, 64, -1);
  textout(bmp, datafile[FONT_GAME].dat, "TAB - changes the active bubble", 8, 84, -1);
  textout(bmp, datafile[FONT_GAME].dat, "ESC - \"fast\" exit... VERY FAST! :-)", 8, 104, -1);

  my_clear_keybuf();
  readkey();
  my_clear_keybuf();

  clear(bmp);
}



void draw_credits(BITMAP *bmp)
{
  static char *credits_text[] = {
     "Donkey Bolonkey - Made In Argentina",
     "In Only 3 Days!!!",
     "",
     "David A. Capello",
     "Programmer, 2d graphics, sounds",
     "",
     "My brothers: Some good ideas for the",
     "graphics, sounds, and the game...",
     "and a lot of \"IMPOSSIBLES\" IDEAS! :-)",
     "",
     "The graphics were made with 2DSS",
     "----------------------------------------------------",
     "This game was created for",
     "the SpeedHack 2001",
     NULL,
  };
  int c;

  blit(get_bitmap(BMP_BG3), bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  text_mode(-1);

  for (c=0; credits_text[c]; c++)
    textout_centre(bmp, datafile[FONT_GAME].dat, credits_text[c],
      SCREEN_W/2, text_height(datafile[FONT_GAME].dat)*c, -1);

  my_clear_keybuf();
  readkey();
  my_clear_keybuf();

  clear(bmp);
}



