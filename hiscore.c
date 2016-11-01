/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "dkbk.h"



#define MAX_HISCORES		10
#define MAX_CHARS_PER_NAME	23



typedef struct HISCORE
{
  char name[MAX_CHARS_PER_NAME];
  int score;
} HISCORE;



static HISCORE hiscore_table[MAX_HISCORES];

static char *hiscore_section = "hiscore";

static char name_string[MAX_CHARS_PER_NAME] = "";
static char *default_name = "David Capello";
static int default_score = 100;

static int hiscore_time = 0;

static int new_hiscore = -1;



void init_hiscore()
{
  char buf[32];
  int i;

  push_config_state();
  set_config_file("dkbk.hi");

  for (i=0; i<MAX_HISCORES; i++) {
    sprintf(buf, "name%d", i+1);
    strncpy(hiscore_table[i].name,
            get_config_string(hiscore_section, buf, default_name),
            sizeof(hiscore_table[i].name)-1);

    sprintf(buf, "score%d", i+1);
    hiscore_table[i].score = get_config_int(hiscore_section, buf, default_score);
  }

  pop_config_state();
}



void shutdown_hiscore()
{
  save_hiscore();
}



void save_hiscore()
{
  char buf[32];
  int i;

  push_config_state();
  set_config_file("dkbk.hi");

  for (i=0; i<MAX_HISCORES; i++) {
    sprintf(buf, "name%d", i+1);
    set_config_string(hiscore_section, buf, hiscore_table[i].name);

    sprintf(buf, "score%d", i+1);
    set_config_int(hiscore_section, buf, hiscore_table[i].score);
  }

  pop_config_state();
}



int update_hiscore()
{
  if (new_hiscore < 0) {
    if (keypressed()) {
      hiscore_time = 0;
      my_clear_keybuf();
      return TRUE;
    }
  }
  else {
    if (keypressed()) {
      int scancode = readkey();
      int ascii = scancode&0xff;

      switch (scancode >> 8) {
        case KEY_ENTER:
          new_hiscore = -1;
          hiscore_time = 0;
          break;

        case KEY_BACKSPACE:
          if (strlen(hiscore_table[new_hiscore].name) > 0) {
            hiscore_table[new_hiscore].name[
              strlen(hiscore_table[new_hiscore].name)-1] = 0;
          }
          break;

        default:
          if (strlen(hiscore_table[new_hiscore].name) < MAX_CHARS_PER_NAME-1) {
            if ((ascii>=32) && (ascii<128)) {
              sprintf(hiscore_table[new_hiscore].name+
                strlen(hiscore_table[new_hiscore].name),
                "%c", ascii);
            }
          }
          break;
      }
    }
  }

  hiscore_time++;
  return FALSE;
}



void draw_hiscore(BITMAP *bmp)
{
  int i, x, y;

  /* background */
  blit(get_bitmap(get_level()->bg), bmp, 0, 0, 0, 0, GAME_W, GAME_H);

  /* title */
  textout_centre(bmp, datafile[FONT_GAME].dat, "HI-SCORES", GAME_W/2, 4, -1);

  /* hiscore table */
  text_mode(-1);
  for (i=0; i<MAX_HISCORES; i++) {
    y = 26+text_height(datafile[FONT_HISCORE].dat)*i;

    textprintf_right(bmp, datafile[FONT_HISCORE].dat, 32, y, -1, "%d.", i+1);

    textprintf(bmp, datafile[FONT_HISCORE].dat, 48, y, -1,
      "%s", hiscore_table[i].name);

    textprintf_right(bmp, datafile[FONT_HISCORE].dat, GAME_W-8, y, -1,
      "%d", hiscore_table[i].score);
  }

  /* message */
  if (new_hiscore < 0) {
    if (hiscore_time > FRAMES_PER_SECOND) {
      if (hiscore_time%FRAMES_PER_SECOND < FRAMES_PER_SECOND/2) {
        text_mode(-1);
        textout_centre(bmp, datafile[FONT_GAME].dat,
          "Press any key to continue", GAME_W/2, GAME_H-40, -1);
      }
    }
  }
  /* cursor */
  else {
    if (hiscore_time%FRAMES_PER_SECOND < FRAMES_PER_SECOND/2) {
      x = 48+text_length(datafile[FONT_HISCORE].dat, hiscore_table[new_hiscore].name);
      y = 26+text_height(datafile[FONT_HISCORE].dat)*(new_hiscore+1);
      rectfill(bmp, x, y-2, x+8, y, makecol(255, 255, 255));
      rect(bmp, x-1, y-2-1, x+8+1, y+1, makecol(0, 0, 0));
    }
  }
}



void add_hiscore(int score)
{
  int i, j;

  for (i=0; i<MAX_HISCORES; i++) {
    if (score > hiscore_table[i].score) {
      for (j=MAX_HISCORES-1; j>i; j--)
        hiscore_table[j] = hiscore_table[j-1];

      new_hiscore = i;
      strcpy(hiscore_table[i].name, name_string);
      hiscore_table[i].score = score;
      break;
    }
  }
}
