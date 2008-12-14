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

#include <time.h>
#include <stdlib.h>
#include <allegro.h>
#include <allegro/internal/aintern.h>
#include "dkbk.h"



/* unique global variables */
DATAFILE *datafile = NULL;
COLOR_MAP *tint_map = NULL;
struct _funnel_s funnel;
struct _crusher_s crusher;



static int count;

static void inc_count()
{
  count++;
}

END_OF_STATIC_FUNCTION(inc_count);



static void play_game()
{
  BITMAP *bmp = create_bitmap(SCREEN_W, SCREEN_H);
  int gameover = FALSE;

  init_level();
  init_donkeys();
  init_particles();
  init_crusher();
  init_banner();
  init_player();

  LOCK_VARIABLE(count);
  LOCK_FUNCTION(inc_count);

  count = 0;
  install_int_ex(inc_count, BPS_TO_TIMER(FRAMES_PER_SECOND));

  my_clear_keybuf();

  while (!gameover) {
    while ((count > 0) && (!gameover)) {
      poll_keyboard();

      update_level();
      update_donkeys();
      update_particles();
      update_crusher();
      update_banner();

      gameover = update_player();

      count--;
    }

    clear(bmp);

    draw_level(bmp);
    draw_donkeys(bmp);
    draw_particles_first(bmp);
    draw_crusher(bmp);
    draw_banner(bmp);
    draw_particles_last(bmp);
    draw_player(bmp);

    vsync();
    blit(bmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    /* create new capture */
    if (key[KEY_F12])
      create_capture(bmp);

    /* pause */
    if (key[KEY_P]) {
      int old_count = count;

      textout_centre(screen, datafile[FONT_GAME].dat, "[ PAUSE ]",
        SCREEN_W/2, SCREEN_H/3, -1);

      my_clear_keybuf();
      readkey();
      my_clear_keybuf();

      count = old_count;
    }
  }

  remove_int(inc_count);

  shutdown_level();
  shutdown_donkeys();
  shutdown_particles();
  shutdown_crusher();
  shutdown_banner();
  shutdown_player();

  destroy_bitmap(bmp);
}



static int show_final_state;



void show_final()
{
  show_final_state = TRUE;
}



void my_clear_keybuf(void)
{
  int c;

  clear_keybuf();

  for (c=0; c<KEY_MAX; c++) {
    key[c] = 0;
    _key[c] = 0;
  }
}



int main(int argc, char *argv[])
{
  int first = TRUE;

  (void)argc;
  (void)argv;

  srand(time(NULL));

  allegro_init();

  set_color_depth(8);
  if (set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0) < 0) {
    allegro_message("Error setting graphics mode\n");
    return 1;
  }

  install_timer();
  install_keyboard();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

  init_graphics();
  init_hiscore();

  draw_warning(screen);

  while (title_screen()) {
    if (first) {
      draw_controls(screen);
      first = FALSE;
    }

    show_final_state = FALSE;
    play_game();
    if (show_final_state)
      draw_credits(screen);

    active_hiscore();
  }

  shutdown_graphics();
  shutdown_hiscore();
  return 0;
}

END_OF_MAIN();

