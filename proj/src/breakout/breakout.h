#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"

typedef struct {
  Sprite *bar;
  Sprite *ball;
  Sprite *bricks[60];
  bool active_bricks[60];
  int lives;
  int score;
} breakout_t;

breakout_t* breakout_init();

void create_bricks(breakout_t *breakout);

int draw_bricks(breakout_t *breakout);

int draw_lives(breakout_t *breakout);

int draw_score(breakout_t *breakout);

int draw_breakout(breakout_t *breakout);

void destroy_breakout(breakout_t *breakout);

void handle_ball_collisions(breakout_t *breakout);

#endif
