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



#define BANNER_X \
  ((crusher.x+crusher.w/2-get_bitmap(BMP_CRUSHER)->w/2+ \
    get_bitmap(BMP_CRUSHER)->w)+108)

#define BANNER_Y \
  ((crusher.y+crusher.h/2-get_bitmap(BMP_CRUSHER)->h/2+8+ \
    get_bitmap(BMP_CRUSHER)->h-get_bitmap(BMP_SCORER)->h)+6)

#define BANNER_W   75
#define BANNER_H   28



static int banner_time;
static int draw_banner_time;
static int banner_type;



void init_banner()
{
  banner_time = 0;
  draw_banner_time = -1;
  banner_type = BMP_BANNER5;
}



void shutdown_banner()
{
}



void update_banner()
{
  banner_time++;
  if (draw_banner_time >= 0)
    draw_banner_time++;

  if (banner_time > FRAMES_PER_SECOND*6) {
    banner_time = 0;
    draw_banner_time = 0;
    switch (banner_type) {
      case BMP_BANNER1: banner_type = BMP_BANNER2; break;
      case BMP_BANNER2: banner_type = BMP_BANNER3; break;
      case BMP_BANNER3: banner_type = BMP_BANNER4; break;
      case BMP_BANNER4: banner_type = BMP_BANNER5; break;
      case BMP_BANNER5: banner_type = BMP_BANNER1; break;
    }
  }

  if (draw_banner_time > FRAMES_PER_SECOND*5/2)
    draw_banner_time = -1;
}



void draw_banner(BITMAP *bmp)
{
  int x1, y1, x2, y2;
  int x, y, f, gray;

  if (draw_banner_time >= 0) {
    x1 = BANNER_X;
    y1 = BANNER_Y;
    x2 = x1+BANNER_W-1;
    y2 = y1+BANNER_H-1;

    /* with interference */
    if (draw_banner_time < FRAMES_PER_SECOND) {
      /* draw the banner */
      blit(get_bitmap(banner_type), bmp, 0, 0, x1, y1, x2-x1+1, y2-y1+1);

      /* draw the interference */
      f = 0xff - 0xff * draw_banner_time / FRAMES_PER_SECOND;
      for (y=y1; y<=y2; y++)
        for (x=x1; x<=x2; x++)
          if ((rand()&0xff) < f) {
            gray = rand()&0xff;
            putpixel(bmp, x, y, makecol(gray, gray, gray));
          }
    }
    else if (draw_banner_time < FRAMES_PER_SECOND*2) {
      /* draw the banner */
      blit(get_bitmap(banner_type), bmp, 0, 0, x1, y1, x2-x1+1, y2-y1+1);
    }
    else {
      f = 100 * (draw_banner_time-FRAMES_PER_SECOND*2) / (FRAMES_PER_SECOND/8);

      stretch_blit(get_bitmap(banner_type), bmp, 0, 0, BANNER_W, BANNER_H,
        x1, y1+BANNER_H/2*f/100,
        x2-x1+1, y2-y1+1-BANNER_H*f/100);
    }
  }
}




