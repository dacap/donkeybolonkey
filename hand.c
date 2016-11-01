/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#include <allegro.h>
#include "dkbk.h"



static int hand_count;
static struct {
  int x, y, color;
} hand[DONKEY_COLOR_MAX];

static int primary_color;



/* call this before `good_hand' */
void get_hand()
{
  DONKEY *donkey;
  int x, y, c;

  hand_count=0;
  for (y=0; y<LEVEL_HEIGHT; y++)
    for (x=0; x<LEVEL_WIDTH; x++)
      if (get_block_flags(x, y) & BLOCK_FLAG_EXIT) {
        donkey = get_block_donkey(x, y);
        hand[hand_count].x = x;
        hand[hand_count].y = y;
        hand[hand_count].color = (donkey)? donkey->color: -1;
        hand_count++;
      }

  primary_color = DONKEY_COLOR_JOKER;
  for (c=0; c<hand_count; c++)
    if (hand[c].color != DONKEY_COLOR_JOKER) {
      primary_color = hand[c].color;
      break;
    }
}



int hand_state()
{
  int c;

  for (c=0; c<hand_count; c++) {
    if (hand[c].color < 0)
      return HAND_GOOD;

    if (hand[c].color != primary_color) {
      if (c >= 3)
        return HAND_FULL;
      else
        return HAND_BAD;
    }
  }

  return HAND_FULL;
}



void kill_hand()
{
  DONKEY *donkey;
  int c, score;

  /* add the correct score */
  score = 0;
  for (c=0; c<hand_count; c++) {
    if (hand[c].color != primary_color)
      break;
  }

  score = (c == 3)? 50:
          (c == 4)? 150:
          (c == 5)? 400:
          (c == 6)? 1600: 0;

  countup_score_player(score);

  /* add the level door animation */
  open_doors(c);

  /* delete the donkeys */
  for (c=0; c<hand_count; c++) {
    donkey = get_block_donkey(hand[c].x, hand[c].y);
    if (donkey) {
      if (donkey->color == primary_color) {
        add_death_donkey(donkey, (c<hand_count/2)? TRUE: FALSE);
        kill_donkey(donkey);
        set_block_donkey(hand[c].x, hand[c].y, NULL);
        continue;
      }
    }
    break;
  }

  hand_count=0;

  /* scream of the donkeys */
  play_sample(datafile[SAMP_DONKEY_SCREAM].dat, 255, 128, 1000, FALSE);
}



static int internal_counter;



int get_first_hand_pos(int *x, int *y)
{
  internal_counter = 0;
  return get_next_hand_pos(x, y);
}



int get_next_hand_pos(int *x, int *y)
{
  if (internal_counter == hand_count)
    return TRUE;

  *x = hand[internal_counter].x;
  *y = hand[internal_counter].y;
  internal_counter++;

  return FALSE;
}
