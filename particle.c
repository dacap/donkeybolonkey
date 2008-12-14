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



#define MAX_PARTICLES		1024
#define MAX_PARTICLES_CHAINS	4

#define PARTICLE_TYPE_BLOOD	1
#define PARTICLE_TYPE_BODY1	2
#define PARTICLE_TYPE_BODY2	3
#define PARTICLE_TYPE_BODY3	4



typedef struct PARTICLE
{
  int type;
  int time;
  int x, y;
  float xspeed, yspeed;
  int size;
  float weight;
} PARTICLE;



typedef struct PARTICLE_CHAIN
{
  int time;
  int x, y;
  int num;
  int cant;
} PARTICLE_CHAIN;



static PARTICLE *particle;
static int particle_count;

static PARTICLE_CHAIN *particle_chain;
static int particle_chain_count;



static void get_particle_position(int i, int *x, int *y)
{
  float time;

  time = (float)6.0 * particle[i].time / FRAMES_PER_SECOND;

  *x = particle[i].x + particle[i].xspeed*time;
  *y = particle[i].y - particle[i].yspeed*time + particle[i].weight*time*time;
}


    
void init_particles()
{
  particle = malloc(sizeof(PARTICLE) * MAX_PARTICLES);
  particle_chain = malloc(sizeof(PARTICLE_CHAIN) * MAX_PARTICLES_CHAINS);

  particle_count = 0;
  particle_chain_count = 0;
}



void shutdown_particles()
{
  free(particle);
  free(particle_chain);
}



void update_particles()
{
  int i, j, x, y;

  /* particles */
  for (i=0; i<particle_count; i++) {
    particle[i].time++;

    get_particle_position(i, &x, &y);
    if ((x < -4) || (x >= GAME_W+4) || (y >= GAME_H+4)) {
      /* delete particle */
      for (j=i; j<particle_count; j++)
        particle[j] = particle[j+1];
      particle_count--;
      i--;
    }
  }

  /* particles chains */
  for (i=0; i<particle_chain_count; i++) {
    particle_chain[i].time++;
    if (particle_chain[i].time > FRAMES_PER_SECOND/8) {
      particle_chain[i].time = 0;

      if (particle_chain[i].num > 0) {
        /* create a new blood spurt */
        for (j=0; j<particle_chain[i].cant; j++)
          create_blood_particle(particle_chain[i].x, particle_chain[i].y);

        create_body_particle(particle_chain[i].x, particle_chain[i].y, FALSE);
        create_body_particle(particle_chain[i].x, particle_chain[i].y, TRUE);

        particle_chain[i].num--;
      }
      else {
        /* delete particle */
        for (j=i; j<particle_chain_count; j++)
          particle_chain[j] = particle_chain[j+1];
        particle_chain_count--;
        i--;
      }
    }
  }
}



static void draw_particles(BITMAP *bmp, int comp)
{
  int i, x, y, size;
  int red;

  red = makecol(255, 0, 0);

  for (i=0; i<particle_count; i++) {
    if (comp) {
      if (particle[i].time < FRAMES_PER_SECOND/4)
        continue;
    }
    else {
      if (particle[i].time >= FRAMES_PER_SECOND/4)
        continue;
    }

    get_particle_position(i, &x, &y);

    switch (particle[i].type) {

      case PARTICLE_TYPE_BLOOD:
        size = particle[i].size * particle[i].time / (FRAMES_PER_SECOND*2);
        if (size == 0)
          putpixel(bmp, x, y, red);
        else
          circlefill(bmp, x, y, size, red);
        break;

      case PARTICLE_TYPE_BODY1:
      case PARTICLE_TYPE_BODY2:
      case PARTICLE_TYPE_BODY3:
        {
          BITMAP *sprite =
	    (particle[i].type == PARTICLE_TYPE_BODY2)? get_bitmap(BMP_DONKEY_PART2):
	    (particle[i].type == PARTICLE_TYPE_BODY3)? get_bitmap(BMP_DONKEY_PART3):
						       get_bitmap(BMP_DONKEY_PART1);
          rotate_sprite(bmp, sprite, x, y,
            itofix(256) * (particle[i].time%FRAMES_PER_SECOND) / FRAMES_PER_SECOND);
        }
        break;
    }
  }
}



void draw_particles_first(BITMAP *bmp)
{
  draw_particles(bmp, FALSE);
}



void draw_particles_last(BITMAP *bmp)
{
  draw_particles(bmp, TRUE);
}



void create_blood_particle(int x, int y)
{
  float getx() {
    float x;
    for(ever) {
      x = (rand()%5001)/50.0 - 25.0;
      if (x > 0.0) {
        if ((rand()&0xff) <= (0xff*+x/25))
          continue;
      }
      else if (x < 0.0) {
        if ((rand()&0xff) <= (0xff*-x/25))
          continue;
      }
      break;
    }
    return x;
  }

  if (particle_count < MAX_PARTICLES) {
    particle[particle_count].type = PARTICLE_TYPE_BLOOD;
    particle[particle_count].time = 0;
    particle[particle_count].x = x;
    particle[particle_count].y = y;
    particle[particle_count].xspeed = getx();
    particle[particle_count].yspeed = 40+(rand()%21)-10;
    particle[particle_count].size = rand()%4;
    particle[particle_count].weight = 4.0+(rand()%20)/10.0;
    particle_count++;
  }
}



void create_blood_particle_in_chain(int x, int y, int num, int cant)
{
  if (particle_chain_count < MAX_PARTICLES_CHAINS) {
    particle_chain[particle_chain_count].time = FRAMES_PER_SECOND;
    particle_chain[particle_chain_count].x = x;
    particle_chain[particle_chain_count].y = y;
    particle_chain[particle_chain_count].num = num;
    particle_chain[particle_chain_count].cant = cant*2;
    particle_chain_count++;
  }
}



void create_body_particle(int x, int y, int accept_head)
{
  float getx() {
    float x;
    for(ever) {
      x = (rand()%5001)/50.0 - 25.0;
      if (x > 0.0) {
        if ((rand()&0xff) <= (0xff*+x/25))
          continue;
      }
      else if (x < 0.0) {
        if ((rand()&0xff) <= (0xff*-x/25))
          continue;
      }
      break;
    }
    return x;
  }

  if (particle_count < MAX_PARTICLES) {
    int c = rand()%3;

    particle[particle_count].type =
      ((c == 2) && (accept_head))? PARTICLE_TYPE_BODY3:
      (c == 1)? PARTICLE_TYPE_BODY2: PARTICLE_TYPE_BODY1;

    particle[particle_count].time = 0;
    particle[particle_count].x = x;
    particle[particle_count].y = y;
    particle[particle_count].xspeed = getx();
    particle[particle_count].yspeed = 60+(rand()%21)-10;
    particle[particle_count].size = rand()%4;
    particle[particle_count].weight = 4.0+(rand()%20)/10.0;
    particle_count++;
 }
}




