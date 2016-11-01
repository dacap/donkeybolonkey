/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#include <stdio.h>
#include <allegro.h>
#include "dkbk.h"



void init_graphics()
{
  RGB *pal;
  int i, j;

  datafile = load_datafile("dkbk.dat");
  tint_map = malloc(sizeof(COLOR_MAP));

  /* check the data file and memory */
  if ((!datafile) || (!tint_map))
    exit(1);

  /* change screen palette */
  pal = datafile[PAL_MAIN].dat;
  pal[0].r = pal[0].g = pal[0].b = 0;
  set_palette(pal);

  /* create the tint table */
  for (i=0; i<DONKEY_COLOR_MAX; i++) {
    for (j=0; j<PAL_SIZE; j++)
      tint_map->data[i][j] = j;

    for (j=16; j<32; j++)
      tint_map->data[i][j] = j+i*16;
  }
}



void shutdown_graphics()
{
  if (datafile)
    unload_datafile(datafile);

  if (tint_map)
    free(tint_map);
}



void create_capture(BITMAP *bmp)
{
  PALETTE pal;
  char buf[256];
  int c;

  for (c=0; c<1000; c++) {
    sprintf(buf, "capt%04d.pcx", c);
    if (!exists(buf))
      break;
  }

  get_palette(pal);
  save_bitmap(buf, bmp, pal);

  do {
    poll_keyboard();
  } while (key[KEY_F12]);
}
