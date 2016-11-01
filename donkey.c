/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#include <allegro.h>
#include "dkbk.h"



#define get_block_flags2(x,y)     get_block_flags(x/2, y/2)
#define get_block_donkey2(x,y)    get_block_donkey(x/2, y/2)
#define set_block_donkey2(x,y,d)  set_block_donkey(x/2, y/2, d)
#define get_level_home2(x, y)     {get_level_home(x, y); *x*=2; *y*=2; }



static DONKEY *donkey_list;

static int move_freq; /* milliseconds */
static int move_time; /* 0 to move_freq */

static int add_freq;  /* move times (2 blocks, 3 blocks...) */
static int add_time;  /* 0 to add_freq */


static struct DEATH_DONKEY
{
  int time;
  int x, y;
  int color;
  int clockwise;
  int flip;
} death_donkey[DONKEY_COLOR_MAX*2];

static int death_donkey_count;



void init_donkeys()
{
  /* donkey list to NULL */
  donkey_list = NULL;

  /* for move donkeys */
  move_freq = get_level()->donkey.move_freq_max;
  move_time = move_freq;

  /* for add donkeys */
  add_freq = get_level()->donkey.add_freq;
  add_time = add_freq;

  death_donkey_count = 0;
}



void shutdown_donkeys()
{
  DONKEY *pos, *next;

  for (pos=donkey_list; pos; pos=next) {
    next = pos->next;
    free(pos);
  }

  donkey_list = NULL;
}



static void move_donkey_position(DONKEY *donkey, int flags)
{
  if (flags & BLOCK_FLAG_LEFT)
    donkey->x-=2;

  if (flags & BLOCK_FLAG_UP)
    donkey->y-=2;

  if (flags & BLOCK_FLAG_RIGHT)
    donkey->x+=2;

  if (flags & BLOCK_FLAG_DOWN)
    donkey->y+=2;
}



static void move_donkeys()
{
  DONKEY *donkey;
  int new_flags;
  int flags;
  int x, y;

  /* see the `hand' */

  get_hand();
  if (hand_state() == HAND_FULL) {
    kill_hand();
    get_hand();
    /* speed up */
    move_freq -= get_level()->donkey.move_freq_countdown;
    if (move_freq < get_level()->donkey.move_freq_min)
      move_freq = get_level()->donkey.move_freq_min;
  }

  /* move the donkeys... */

  /* EXIT */
  /* go to first `exit' */
  if (!get_first_hand_pos(&x, &y)) {
    for(ever) {
      /* get the donkey in the block */
      donkey = get_block_donkey(x, y);
      /* ok? */
      if (donkey) {
        flags = get_block_flags2(donkey->x, donkey->y);

        donkey->xold = donkey->x;
        donkey->yold = donkey->y;

        move_donkey_position(donkey, flags);

        /* the flags of the block to come */
        new_flags = get_block_flags2(donkey->x, donkey->y);

        /* next position not is a `exit' block */
        if (!(new_flags & BLOCK_FLAG_EXIT)) {
          if (hand_state() != HAND_BAD) {
            /* return to old position */
            donkey->x = donkey->xold;
            donkey->y = donkey->yold;
          }
        }
        else {
          if ((get_block_donkey2(donkey->x, donkey->y)) &&
              (get_block_donkey2(donkey->x, donkey->y) != donkey)) {
            if (hand_state() != HAND_BAD) {
              /* return to old position */
              donkey->x = donkey->xold;
              donkey->y = donkey->yold;
            }
          }
        }

        /* restore `donkey' pointers in the block */
        if (get_block_donkey2(donkey->xold, donkey->yold) == donkey)
          set_block_donkey2(donkey->xold, donkey->yold, NULL);

        set_block_donkey2(donkey->x, donkey->y, donkey);
        donkey->moved = TRUE;
        donkey->flip = (donkey->xold == donkey->x)? donkey->flip:
                       (donkey->xold >  donkey->x)? TRUE: FALSE;
      }

      /* go to next `exit' */
      if (get_next_hand_pos(&x, &y))
        break;
    }
  }

  /* OTHERS */
  for (donkey=donkey_list; donkey; donkey=donkey->next) {
    if (donkey->moved)
      continue;

    flags = get_block_flags2(donkey->x, donkey->y);

    if (flags & BLOCK_FLAG_STOP)
      continue;

    donkey->xold = donkey->x;
    donkey->yold = donkey->y;

    move_donkey_position(donkey, flags);

    /* restore `donkey' pointers in the block */
    if (get_block_donkey2(donkey->xold, donkey->yold) == donkey)
      set_block_donkey2(donkey->xold, donkey->yold, NULL);

    set_block_donkey2(donkey->x, donkey->y, donkey);
    donkey->moved = TRUE;
    donkey->flip = (donkey->xold == donkey->x)? donkey->flip:
                   (donkey->xold >  donkey->x)? TRUE: FALSE;
  }

  /* STOP */
  for (donkey=donkey_list; donkey; donkey=donkey->next) {
    if (donkey->moved) {
      donkey->moved = FALSE;
      continue;
    }

    flags = get_block_flags2(donkey->x, donkey->y);

    donkey->xold = donkey->x;
    donkey->yold = donkey->y;

    move_donkey_position(donkey, flags);

    /* is new position occupied? */
    if ((get_block_donkey2(donkey->x, donkey->y)) &&
        (get_block_donkey2(donkey->x, donkey->y) != donkey)) {
      /* if yes... return to old position */
      donkey->x = donkey->xold;
      donkey->y = donkey->yold;

      active_red_alarm(100);
    }

    /* restore `donkey' pointers in the block */
    if (get_block_donkey2(donkey->xold, donkey->yold) == donkey)
      set_block_donkey2(donkey->xold, donkey->yold, NULL);

    set_block_donkey2(donkey->x, donkey->y, donkey);
    donkey->flip = (donkey->xold == donkey->x)? donkey->flip:
                   (donkey->xold >  donkey->x)? TRUE: FALSE;
  }
}



void update_donkeys()
{
  int i, j, c;

  move_time++;
  if (move_time > FRAMES_PER_SECOND*move_freq/1000) {
    move_time = 0;
    move_donkeys();
    add_time++;
  }

  if (add_time > add_freq) {
    add_time = 0;
    add_donkey();
  }

  /* move death donkeys */
  c=0;
  for (i=0; i<death_donkey_count; i++) {
    death_donkey[i].time++;
    if (death_donkey[i].time >= FRAMES_PER_SECOND) {
      c++;
      /* delete death donkey */
      for (j=i; j<death_donkey_count; j++)
        death_donkey[j] = death_donkey[j+1];
      death_donkey_count--;
      i--;
      /* count down the donkeys counter */
      countdown_counter_player();
      active_blue_alarm(200);
    }
  }

  if (c > 0) {
    play_sample(datafile[SAMP_CRUSHER].dat, 255, 128, 1000, FALSE);
    create_blood_particle_in_chain(
      crusher.x+crusher.w/2, crusher.y+BLOCK_HEIGHT, c, c*32);
  }
}



void draw_donkeys(BITMAP *bmp)
{
  int x1, y1, x2, y2;
  DONKEY *donkey;
  int i, x, y;

  /* draw normal donkeys */
  for (donkey=donkey_list; donkey; donkey=donkey->next) {
    draw_a_donkey(bmp,
      (move_time) < ((FRAMES_PER_SECOND/2)*move_freq/1000)?
        get_bitmap(BMP_DONKEY): get_bitmap(BMP_DONKEY_WALK),
      donkey->x*BLOCK_WIDTH/2+BLOCK_WIDTH/2-get_bitmap(BMP_DONKEY)->w/2,
      donkey->y*BLOCK_HEIGHT/2+BLOCK_HEIGHT/2-get_bitmap(BMP_DONKEY)->h/2-4,
      donkey->color, donkey->flip, itofix(1), itofix(1));
  }

  /* draw the death donkeys */
  for (i=0; i<death_donkey_count; i++) {
    if (death_donkey[i].time < FRAMES_PER_SECOND/2) {
      x1 = death_donkey[i].x;
      y1 = death_donkey[i].y;
      x2 = death_donkey[i].x;
      y2 = death_donkey[i].y+BLOCK_HEIGHT;

      x = x1 + (x2-x1) * (death_donkey[i].time) / (FRAMES_PER_SECOND/2);
      y = y1 + (y2-y1) * (death_donkey[i].time) / (FRAMES_PER_SECOND/2);
    }
    else {
      x1 = death_donkey[i].x;
      y1 = death_donkey[i].y+BLOCK_HEIGHT;
      x2 = crusher.x+crusher.w/2-get_bitmap(BMP_DONKEY_DEATH)->w/2;
      y2 = crusher.y+crusher.h/2-get_bitmap(BMP_DONKEY_DEATH)->h/2;//+BLOCK_HEIGHT;

      x = x1 + (x2-x1) * (death_donkey[i].time-(FRAMES_PER_SECOND/2)) / (FRAMES_PER_SECOND/2);
      y = y1 + (y2-y1) * (death_donkey[i].time-(FRAMES_PER_SECOND/2)) / (FRAMES_PER_SECOND/2);
    }

    draw_a_donkey(bmp,
      get_bitmap(BMP_DONKEY_DEATH),
      x+rand()%3-1,
      y+rand()%3-1,
      death_donkey[i].color,
      death_donkey[i].flip,
      (death_donkey[i].time < FRAMES_PER_SECOND/2)?
        0:
        (itofix(256) * (death_donkey[i].time-(FRAMES_PER_SECOND/2)) / (FRAMES_PER_SECOND/2))
        * ((death_donkey[i].clockwise)? 1: -1),
      itofix(1));
  }
}



void draw_a_donkey(BITMAP *bmp, BITMAP *sprite, int x, int y, int color, int flip, fixed angle, fixed scale)
{
  BITMAP *temp = create_bitmap(sprite->w, sprite->h);
  clear(temp);

  ((flip)? draw_sprite_h_flip: draw_sprite)(temp, sprite, 0, 0);

  color_map = tint_map;
  draw_lit_sprite(temp, temp, 0, 0, color);
  color_map = NULL;

  if ((angle == itofix(1)) && (scale == itofix(1)))
    draw_sprite(bmp, temp, x, y);
  else
    rotate_scaled_sprite(bmp, temp, x, y, angle, scale);

  destroy_bitmap(temp);
}



void add_donkey()
{
  DONKEY *donkey;

  /* create the new donkey */
  donkey = malloc(sizeof(DONKEY));

  /* default flip */
  donkey->flip = FALSE;

  /* take the first position */
  get_level_home2(&donkey->x, &donkey->y);
  donkey->xold = donkey->x;
  donkey->yold = donkey->y;

  /* get a color for the donkey */
  donkey->color = get_level_random_color();

  /* `moved' to FALSE */
  donkey->moved = FALSE;

  /* next donkey to NULL, because this is the last of the list */
  donkey->next = NULL;

  /* check the home */
  if (get_block_donkey(donkey->x/2, donkey->y/2)) {
    /* is full... */

    /* destroy the new donkey */
    free(donkey);

    /* kill the player */
    kill_player();
  }
  else {
    /* is empty... */

    /* to occupy the position in the level */
    set_block_donkey(donkey->x/2, donkey->y/2, donkey);

    /* add donkey to list */
    add_donkey_to_list(donkey);
  }
}



void kill_donkey(DONKEY *donkey)
{
  remove_donkey_from_list(donkey);
  free(donkey);
}



void add_death_donkey(DONKEY *donkey, int clockwise)
{
  death_donkey[death_donkey_count].time = 0;
  death_donkey[death_donkey_count].x = donkey->x*BLOCK_WIDTH/2+BLOCK_WIDTH/2-get_bitmap(BMP_DONKEY)->w/2;
  death_donkey[death_donkey_count].y = donkey->y*BLOCK_HEIGHT/2+BLOCK_HEIGHT/2-get_bitmap(BMP_DONKEY)->h/2-4;
  death_donkey[death_donkey_count].color = donkey->color;
  death_donkey[death_donkey_count].clockwise = clockwise;
  death_donkey[death_donkey_count].flip = donkey->flip;
  death_donkey_count++;
}



/* adds a new donkey to the list */
void add_donkey_to_list(DONKEY *donkey)
{
  donkey->next = donkey_list;
  donkey_list = donkey;
}



/* removes the donkey to the list and level */
void remove_donkey_from_list(DONKEY *donkey)
{
  DONKEY *pos, *prev;

  prev = NULL;
  for (pos=donkey_list; pos; pos=pos->next) {
    if (pos == donkey) {
      if (!prev)
        donkey_list = donkey->next;
      else
        prev->next = donkey->next;

      /* remove the donkey from level */
      set_block_donkey(donkey->x/2, donkey->y/2, NULL);
      break;
    }
    prev = pos;
  }
}
