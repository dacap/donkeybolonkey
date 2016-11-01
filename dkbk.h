/*
   Donkey Bolonkey
   Copyright (C) 2001 David Capello

   This file is released under the terms of the MIT license.
   Read LICENSE.txt for more information.
*/

#ifndef DKBK_H
#define DKBK_H

#include "data.h"



#define ever			;;



#define FRAMES_PER_SECOND	60

#define GAME_W			320
#define GAME_H			240



#define BLOCK_WIDTH		18
#define BLOCK_HEIGHT		18
#define BLOCK_FLAG_LEFT		1
#define BLOCK_FLAG_UP		2
#define BLOCK_FLAG_RIGHT	4
#define BLOCK_FLAG_DOWN		8
#define BLOCK_FLAG_HOME		16
#define BLOCK_FLAG_EXIT		32
#define BLOCK_FLAG_STOP		64
#define BLOCK_FLAG_BUBBLE	128
#define BLOCK_FLAG_TRAP		256

#define LEVEL_WIDTH		16
#define LEVEL_HEIGHT		9

#define HAND_BAD		0
#define HAND_GOOD		1
#define HAND_FULL		2

#define DONKEY_COLOR_WHITE	0
#define DONKEY_COLOR_RED	1
#define DONKEY_COLOR_ORANGE	2
#define DONKEY_COLOR_YELLOW	3
#define DONKEY_COLOR_GREEN	4
#define DONKEY_COLOR_LIGHTBLUE	5
#define DONKEY_COLOR_BLUE	6
#define DONKEY_COLOR_MAGENT	7
#define DONKEY_COLOR_MAX	8
#define DONKEY_COLOR_JOKER	DONKEY_COLOR_WHITE

#define PLAYER_FLAG_GAMEOVER	1
#define PLAYER_FLAG_LOCK	2
#define PLAYER_FLAG_FINAL	4

#define MAX_BUBBLES		4

#define get_bitmap(id)		((BITMAP *)(datafile[(id)].dat))



struct DONKEY;



typedef struct BLOCK
{
  int flags;
  struct DONKEY *donkey;
} BLOCK;



typedef struct LEVEL
{
  BLOCK block[LEVEL_HEIGHT][LEVEL_WIDTH];
  int counter;
  int color_table[DONKEY_COLOR_MAX];
  struct {
    int move_freq_min;
    int move_freq_max;
    int move_freq_countdown;
    int add_freq;
  } donkey;
  int bg;
} LEVEL;



typedef struct DONKEY
{
  int flip;
  int x, y;
  int xold, yold;
  int color;
  int moved;
  struct DONKEY *next;
} DONKEY;



typedef struct PLAYER
{
  int time;
  int flags;
  int level;
  int counter;
  int score;
  struct {
    int state;
    int x, y;
    struct DONKEY *donkey;
  } bubble[MAX_BUBBLES];
  int active_bubble;
} PLAYER;



/* global variables in `main.c' */
extern DATAFILE *datafile;
extern COLOR_MAP *tint_map;

extern struct _funnel_s {
  int x1, y1, x2, y2;
} funnel;

extern struct _crusher_s {
  int x, y, w, h;
} crusher;



/* `main.c' */
void show_final();
void my_clear_keybuf(void);
void my_flip(BITMAP *bmp);


/* `graphics.c' */
void init_graphics();
void shutdown_graphics();
void create_capture(BITMAP *bmp);


/* `level.c' */
void init_level();
void shutdown_level();
void update_level();
void draw_level(BITMAP *bmp);
int reset_level(int level_num);
LEVEL *get_level();
int get_block_flags(int x, int y);
DONKEY *get_block_donkey(int x, int y);
DONKEY *set_block_donkey(int x, int y, DONKEY *donkey);
void get_level_home(int *xout, int *yout);
int get_level_bubble(int *xout, int *yout, int count);
int get_level_random_color();
void open_doors(int num);


/* `hand.c' */
void get_hand();
int hand_state();
void kill_hand();
int get_first_hand_pos(int *x, int *y);
int get_next_hand_pos(int *x, int *y);


/* `donkey.c' */
void init_donkeys();
void shutdown_donkeys();
void update_donkeys();
void draw_donkeys(BITMAP *bmp);
void draw_a_donkey(BITMAP *bmp, BITMAP *sprite, int x, int y, int color, int flip, fixed angle, fixed scale);
void add_donkey();
void kill_donkey(DONKEY *donkey);
void add_death_donkey(DONKEY *donkey, int clockwise);
void add_donkey_to_list(DONKEY *donkey);
void remove_donkey_from_list(DONKEY *donkey);


/* `crusher.c' */
void init_crusher();
void shutdown_crusher();
void update_crusher();
void draw_crusher(BITMAP *bmp);
void active_blue_alarm(int msec);
void active_red_alarm(int msec);
void active_crazy_alarm();


/* `player.c' */
void init_player();
void shutdown_player();
int update_player();
void draw_player(BITMAP *bmp);
void kill_player();
void countdown_counter_player();
void countup_score_player(int score);


/* `particle.c' */
void init_particles();
void shutdown_particles();
void update_particles();
void draw_particles_first(BITMAP *bmp);
void draw_particles_last(BITMAP *bmp);
void create_blood_particle(int x, int y);
void create_blood_particle_in_chain(int x, int y, int num, int cant);
void create_body_particle(int x, int y, int accept_head);


/* `banner.c' */
void init_banner();
void shutdown_banner();
void update_banner();
void draw_banner(BITMAP *bmp);


/* `hiscore.c' */
void init_hiscore();
void shutdown_hiscore();
void save_hiscore();
int update_hiscore();
void draw_hiscore(BITMAP *bmp);
void add_hiscore(int score);


/* `title.c' */
int title_screen(BITMAP *bmp);
void active_hiscore();
void draw_warning(BITMAP *bmp);
void draw_controls(BITMAP *bmp);
void draw_credits(BITMAP *bmp);



#endif /* #ifndef DKBK_H */
