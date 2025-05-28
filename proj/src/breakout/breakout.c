#include "breakout.h"
#include "../assets/ball_xpm.h"
#include "../assets/bar_xpm.h"
#include "../assets/red_brick_xpm.h"
#include "../assets/orange_brick_xpm.h"
#include "../assets/yellow_brick_xpm.h"
#include "../assets/green_brick_xpm.h"
#include "../assets/blue_brick_xpm.h"

breakout_t *breakout_init() {
  breakout_t *breakout = (breakout_t *) malloc(sizeof(breakout_t));

  if (breakout == NULL) {
    return NULL;
  }

  breakout->bar = create_sprite((xpm_map_t) bar_xpm);
  if (breakout->bar == NULL) {
    destroy_sprite(breakout->bar);
    free(breakout);
    return NULL;
  }

  breakout->bar->x = 350;
  breakout->bar->y = 500;

  for (int i = 0; i < 12; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) red_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return NULL;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + i * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 100;
  }

  for (int i = 12; i < 24; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) orange_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return NULL;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 12) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 130;
  }

  for (int i = 24; i < 36; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) yellow_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return NULL;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 24) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 160;
  }

  for (int i = 36; i < 48; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) green_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return NULL;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 36) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 190;
  }

  for (int i = 48; i < 60; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) blue_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return NULL;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 48) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 220;
  }

  breakout->ball = create_sprite((xpm_map_t) ball_xpm);
  if (breakout->ball == NULL) {
    destroy_sprite(breakout->ball);
    free(breakout);
    return NULL;
  }

  breakout->ball->x = 395;
  breakout->ball->y = 490;
  breakout->ball->xspeed = 3;
  breakout->ball->yspeed = -3;

  return breakout;
}

int draw_bricks(breakout_t *breakout) {
  for (int i = 0; i < 60; i++) {
    if (draw_sprite(breakout->bricks[i], breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
      return 1;
    }
  }
  return 0;
}

int draw_breakout(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return 1;
  }

  if (draw_sprite(breakout->bar, breakout->bar->x, breakout->bar->y) != 0) {
    return 1;
  }

  draw_bricks(breakout);

  if (draw_sprite(breakout->ball, breakout->ball->x, breakout->ball->y) != 0) {
    return 1;
  }

  return 0;
}

void destroy_breakout(breakout_t *breakout) {
  if (breakout == NULL) {
    return;
  }

  if (breakout->bar != NULL) {
    destroy_sprite(breakout->bar);
  }

  for (int i = 0; i < 60; i++) {
    destroy_sprite(breakout->bricks[i]);
  }

  if (breakout->ball != NULL) {
    destroy_sprite(breakout->ball);
  }

  free(breakout);
}

void handle_ball_collisions(breakout_t *breakout) {
  if (breakout == NULL || breakout->ball == NULL || breakout->bar == NULL) {
    return;
  }

  if (breakout->ball->x <= 0 || breakout->ball->x + breakout->ball->width >= vmi_p.XResolution) {
    breakout->ball->xspeed = -breakout->ball->xspeed;
  }
  if (breakout->ball->y <= 0) {
    breakout->ball->yspeed = -breakout->ball->yspeed;
  }
  if (breakout->ball->y + breakout->ball->height >= vmi_p.YResolution) {
    breakout->ball->x = 395;
    breakout->ball->y = 490;
    breakout->ball->xspeed = 3;
    breakout->ball->yspeed = -3;
  }

  if (breakout->ball->y + breakout->ball->height >= breakout->bar->y &&
      breakout->ball->y <= breakout->bar->y + breakout->bar->height &&
      breakout->ball->x + breakout->ball->width >= breakout->bar->x &&
      breakout->ball->x <= breakout->bar->x + breakout->bar->width) {
        if (breakout->ball->y + breakout->ball->height - breakout->ball->yspeed <= breakout->bar->y || breakout->ball->y - breakout->ball->yspeed >= breakout->bar->y + breakout->bar->height) {
          breakout->ball->yspeed = -breakout->ball->yspeed;
        } else {
          breakout->ball->xspeed = -breakout->ball->xspeed;
        }
  }

  for (int i = 0; i < 60; i++) {
    
    if (breakout->ball->y <= breakout->bricks[i]->y + breakout->bricks[i]->height &&
        breakout->ball->y + breakout->ball->height >= breakout->bricks[i]->y &&
        breakout->ball->x + breakout->ball->width >= breakout->bricks[i]->x &&
        breakout->ball->x <= breakout->bricks[i]->x + breakout->bricks[i]->width) {
      if (breakout->ball->y + breakout->ball->height - breakout->ball->yspeed <= breakout->bricks[i]->y ||
          breakout->ball->y - breakout->ball->yspeed >= breakout->bricks[i]->y + breakout->bricks[i]->height) {
        breakout->ball->yspeed = -breakout->ball->yspeed;
      } else {
        breakout->ball->xspeed = -breakout->ball->xspeed;
      }
      break;
    }
  }
}
