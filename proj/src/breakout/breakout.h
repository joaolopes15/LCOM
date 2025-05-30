#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"
#include "../sprite/animated_sprite.h"

typedef struct {
  Sprite *bar;
  Sprite *ball;
  Sprite *bricks[60];
  AnimSprite *red_animated_bricks[12];
  AnimSprite *orange_animated_bricks[12];
  AnimSprite *yellow_animated_bricks[12];
  AnimSprite *green_animated_bricks[12];
  bool active_bricks[60];
  bool brick_animating[60];
  int brick_anim_frames[60];
  int lives;
  int score;
  bool ball_attached;
} breakout_t;

breakout_t* breakout_init();

void create_bricks(breakout_t *breakout);

int draw_bricks(breakout_t *breakout);

int draw_lives(breakout_t *breakout);

int draw_score(breakout_t *breakout);

int draw_breakout(breakout_t *breakout);

void destroy_breakout(breakout_t *breakout);

void handle_ball_collisions(breakout_t *breakout);

void move_bar_with_ball_keyboard(breakout_t *breakout, int direction);

void move_bar_with_ball_mouse(breakout_t *breakout, int target_x, int target_y, int xspeed);

#endif
