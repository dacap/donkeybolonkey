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



static PLAYER player1;
static PLAYER *player;



static void find_bubbles()
{
  int c;

  for (c=0; c<MAX_BUBBLES; c++) {
    if (get_level_bubble(&player->bubble[c].x, &player->bubble[c].y, c)) {
      player->bubble[c].state = TRUE;
      player->bubble[c].donkey = NULL;
    }
    else {
      player->bubble[c].state = FALSE;
    }
  }

  player->active_bubble = 0;
}



void init_player()
{
  clear(screen);
  set_palette_range(datafile[PAL_GAMEOVER].dat, 0x90, 0xff, TRUE);

  player = &player1;

  player->time = 0;
  player->flags = 0;

  player->level = 1;
  reset_level(player->level);
  find_bubbles();

  player->counter = get_level()->counter;
  player->score = 0;
}



void shutdown_player()
{
  add_hiscore(player->score);

  if (player->flags & PLAYER_FLAG_FINAL)
    show_final();
}



static void move_position(int flags, int *x, int *y)
{
  if (flags & BLOCK_FLAG_LEFT)
    (*x)--;

  if (flags & BLOCK_FLAG_UP)
    (*y)--;

  if (flags & BLOCK_FLAG_RIGHT)
    (*x)++;

  if (flags & BLOCK_FLAG_DOWN)
    (*y)++;
}



int update_player()
{
  DONKEY *donkey;
  int c, x, y;

  if (key[KEY_ESC])
    return TRUE;

  if (key[KEY_SPACE]) {
    if (!(player->flags & PLAYER_FLAG_LOCK)) {
      player->flags |= PLAYER_FLAG_LOCK;

      x = player->bubble[player->active_bubble].x;
      y = player->bubble[player->active_bubble].y;
      move_position(get_block_flags(x, y), &x, &y);

      /* get the donkey in the block */
      donkey = get_block_donkey(x, y);

      /* is it something? */
      if (donkey) {
        /* remove the donkey (but, not delete) */
        remove_donkey_from_list(donkey);
      }

      /* the player have a donkey? */
      if (player->bubble[player->active_bubble].donkey) {
        /* put this in the list */
        player->bubble[player->active_bubble].donkey->next = NULL;
        add_donkey_to_list(player->bubble[player->active_bubble].donkey);
      }

      if ((donkey) || (player->bubble[player->active_bubble].donkey))
        play_sample(datafile[SAMP_BUBBLE].dat, 255, 128, 1000, FALSE);

      /* put now the donkey in the bubble to the block */
      set_block_donkey(x, y, player->bubble[player->active_bubble].donkey);

      /* the player got the new donkey */
      player->bubble[player->active_bubble].donkey = donkey;
    }
  }
  else if (key[KEY_TAB]) {
    if (!(player->flags & PLAYER_FLAG_LOCK)) {
      player->flags |= PLAYER_FLAG_LOCK;

      /* search the new bubble to come */
      for (c=player->active_bubble+1; c<MAX_BUBBLES*4; c++) {
        if (player->bubble[c%MAX_BUBBLES].state) {
          player->active_bubble = c%MAX_BUBBLES;
          break;
        }
      }
    }
  }
  else {
    player->flags &= ~PLAYER_FLAG_LOCK;
  }

  /* game over */
  if (player->flags & PLAYER_FLAG_GAMEOVER) {
    player->time++;
    if (key[KEY_ESC])
      return TRUE;

    if (!(player->flags & PLAYER_FLAG_FINAL)) {
      if (key[KEY_ENTER]) {
        player->flags ^= PLAYER_FLAG_GAMEOVER;

        reset_level(player->level);
        find_bubbles();

        player->counter = get_level()->counter;
        player->score = 0;
      }
    }
  }

  return FALSE;
}



void draw_player(BITMAP *bmp)
{
  static fixed scale = 0x10000;
  static int scale_add = FALSE;
  int c, x, y, flags;

  for (c=0; c<MAX_BUBBLES; c++) {
    if (player->bubble[c].state) {
      x = player->bubble[c].x;
      y = player->bubble[c].y;
      move_position(get_block_flags(x, y), &x, &y);
      flags = get_block_flags(x, y);

      /* draw the trapped donkey */
      if (player->bubble[c].donkey) {
        draw_a_donkey(bmp, get_bitmap(BMP_DONKEY_BUBBLE),
          player->bubble[c].x*BLOCK_WIDTH+get_bitmap(BMP_BUBBLE)->w/2-get_bitmap(BMP_DONKEY_BUBBLE)->w/2,
          player->bubble[c].y*BLOCK_HEIGHT+get_bitmap(BMP_BUBBLE)->h/2-get_bitmap(BMP_DONKEY_BUBBLE)->h/2-3,
          player->bubble[c].donkey->color,
          player->bubble[c].donkey->flip, itofix(1),
          (c == player->active_bubble)? scale: itofix(1));
      }

      /* draw the bubble */
      pivot_scaled_sprite(bmp, get_bitmap(BMP_BUBBLE),
        player->bubble[c].x*BLOCK_WIDTH+get_bitmap(BMP_BUBBLE)->w/2,
        player->bubble[c].y*BLOCK_HEIGHT+get_bitmap(BMP_BUBBLE)->h/2,
        get_bitmap(BMP_BUBBLE)->w/2,
        get_bitmap(BMP_BUBBLE)->h/2,
        itofix(1), (c == player->active_bubble)? scale: itofix(1));

      if (scale_add) {
        scale+=0x500;
        if (scale > 0x12000)
          scale_add = FALSE;
      }
      else {
        scale-=0x500;
        if (scale < 0x0e000)
          scale_add = TRUE;
      }
    }
  }

  /* scorer */
  text_mode(-1);

  x = crusher.x+crusher.w/2-get_bitmap(BMP_CRUSHER)->w/2;
  y = crusher.y+crusher.h/2-get_bitmap(BMP_CRUSHER)->h/2+8;

  textprintf(bmp, datafile[FONT_NUMBERS].dat,
    x+get_bitmap(BMP_CRUSHER)->w+58,
    y+get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h+63,
    -1, "%03d", player->counter);

  textprintf(bmp, datafile[FONT_NUMBERS].dat,
    x+get_bitmap(BMP_CRUSHER)->w+106,
    y+get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h+63,
    -1, "%08d", player->score);

  /* game over */
  if (player->flags & PLAYER_FLAG_GAMEOVER) {
    draw_sprite(bmp, get_bitmap(BMP_GAMEOVER),
      SCREEN_W/2-get_bitmap(BMP_GAMEOVER)->w/2,
      SCREEN_H/2-get_bitmap(BMP_GAMEOVER)->h/2);

    if (player->time%(FRAMES_PER_SECOND*2) > FRAMES_PER_SECOND) {
      text_mode(-1);

      if (!(player->flags & PLAYER_FLAG_FINAL))
        textout_centre(bmp, datafile[FONT_GAME].dat,
          "Press ENTER to continue", SCREEN_W/2, SCREEN_H-84, -1);

      textout_centre(bmp, datafile[FONT_GAME].dat,
        "Press ESC to exit", SCREEN_W/2, SCREEN_H-60, -1);
    }
  }
}



void kill_player()
{
  player->flags |= PLAYER_FLAG_GAMEOVER;
}



void countdown_counter_player()
{
  if (!(player->flags & PLAYER_FLAG_GAMEOVER))
    player->counter--;

  if (player->counter <= 0) {
    player->counter = 0;

    if (!reset_level(++player->level)) {
      kill_player();
      player->flags |= PLAYER_FLAG_FINAL;
    }
    else {
      find_bubbles();

      player->counter = get_level()->counter;
      active_crazy_alarm();
    }
  }
}



void countup_score_player(int score)
{
  if (!(player->flags & PLAYER_FLAG_GAMEOVER))
    player->score += score;
}




