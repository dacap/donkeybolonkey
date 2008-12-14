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



static int alarm_time_blue;
static int alarm_msec_blue;
static int alarm_time_red;
static int alarm_msec_red;
static int alarm_crazy_time;
static int alarm_crazy_switch;



void init_crusher()
{
  crusher.w = 32;
  crusher.h = 32;
  crusher.x = funnel.x1+(funnel.x2-funnel.x1)/2 - crusher.w/2;
  crusher.y = GAME_H - crusher.h - 8;

  alarm_time_blue = -1;
  alarm_time_red = -1;
  alarm_crazy_time = -1;

  play_sample(datafile[SAMP_CRUSHER_MOTOR].dat, 255, 128, 1000, TRUE);
}



void shutdown_crusher()
{
  stop_sample(datafile[SAMP_CRUSHER_MOTOR].dat);
  stop_sample(datafile[SAMP_ALARM].dat);
}



void update_crusher()
{
  if (alarm_crazy_time < 0) {
    if (alarm_time_blue >= 0) {
      alarm_time_blue++;
      if (1000*alarm_time_blue/FRAMES_PER_SECOND > alarm_msec_blue)
        alarm_time_blue = -1;
    }

    if (alarm_time_red >= 0) {
      alarm_time_red++;
      if (1000*alarm_time_red/FRAMES_PER_SECOND > alarm_msec_red)
        alarm_time_red = -1;
    }
  }
  else {
    alarm_crazy_time++;
    if (alarm_crazy_time > FRAMES_PER_SECOND*3/2) {
      stop_sample(datafile[SAMP_ALARM].dat);
      alarm_crazy_time = -1;
      alarm_time_blue = -1;
      alarm_time_red = -1;
    }
    else {
      alarm_crazy_switch++;
      if (alarm_crazy_switch > FRAMES_PER_SECOND/8) {
        alarm_crazy_switch = 0;

        if (alarm_time_blue >= 0) {
          alarm_time_blue = -1;
          alarm_time_red = 0;
        }
        else {
          alarm_time_blue = 0;
          alarm_time_red = -1;
        }
      }
    }
  }
}



static void strap_proc(BITMAP *bmp, int x, int y, int c)
{
  static int color = 0;
  (void)c;
  putpixel(bmp, x, y, (color&2)? makecol(64, 64, 64): makecol(0, 0, 0));
  color++;
}



void draw_crusher(BITMAP *bmp)
{
  static fixed angle = 0;
  int x, y, x1, y1, x2, y2;

  x = crusher.x+crusher.w/2-get_bitmap(BMP_CRUSHER)->w/2;
  y = crusher.y+crusher.h/2-get_bitmap(BMP_CRUSHER)->h/2+8;

  /* crusher body */
  draw_sprite(bmp, get_bitmap(BMP_CRUSHER), x, y);

  /* crusher motor */
  draw_sprite(bmp, get_bitmap(BMP_CRUSHER_MOTOR),
    x1 = x-get_bitmap(BMP_CRUSHER_MOTOR)->w+1+rand()%3-1,
    y1 = y+rand()%3-1);

  /* crusher pulley */
  rotate_sprite(bmp, get_bitmap(BMP_CRUSHER_PULLEY),
    x2 = x+17-get_bitmap(BMP_CRUSHER_PULLEY)->w/2+rand()%3-1,
    y2 = y+13-get_bitmap(BMP_CRUSHER_PULLEY)->h/2+rand()%3-1,
    angle);

  /* strap */
  do_line(bmp, x1+12, y1+8,
    x2+get_bitmap(BMP_CRUSHER_PULLEY)->w/2-1, y2,
    makecol(0, 0, 0), strap_proc);

  do_line(bmp, x1+12, y1+18,
    x2+get_bitmap(BMP_CRUSHER_PULLEY)->w/2-1,
    y2+get_bitmap(BMP_CRUSHER_PULLEY)->h-1,
    makecol(0, 0, 0), strap_proc);

  angle = fadd(angle, itofix(16));
  if (angle > itofix(256))
    angle = fsub(angle, itofix(256));

  /* scorer */
  draw_sprite(bmp, get_bitmap(BMP_SCORER),
    x+get_bitmap(BMP_CRUSHER)->w,
    y+get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h);

  /* alarm */
  if (alarm_time_blue >= 0) {
    masked_blit(get_bitmap(BMP_SCORER_ALARM), bmp, 0, 0,
      x+get_bitmap(BMP_CRUSHER)->w+63,
      y+get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h+27, 8, 9);
  }

  if (alarm_time_red >= 0) {
    masked_blit(get_bitmap(BMP_SCORER_ALARM), bmp,
      get_bitmap(BMP_SCORER_ALARM)->w-8, 0,
      x+get_bitmap(BMP_CRUSHER)->w+75,
      y+get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h+27, 8, 9);
  }
}



void active_blue_alarm(int msec)
{
  if (alarm_crazy_time < 0) {
    alarm_time_blue = 0;
    alarm_msec_blue = msec;
  }
}



void active_red_alarm(int msec)
{
  if (alarm_crazy_time < 0) {
    alarm_time_red = 0;
    alarm_msec_red = msec;
    play_sample(datafile[SAMP_ALARM].dat, 64, 128, 1000, FALSE);
  }
}



void active_crazy_alarm()
{
  play_sample(datafile[SAMP_ALARM].dat, 255, 128, 1000, TRUE);
  alarm_crazy_time = 0;
  alarm_crazy_switch = 0;
  alarm_time_blue = 0;
  alarm_time_red = -1;
}



