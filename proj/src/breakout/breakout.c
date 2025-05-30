#include "breakout.h"
#include "../assets/ball_xpm.h"
#include "../assets/bar_xpm.h"
#include "../assets/blue_brick_xpm.h"
#include "../assets/green_brick_xpm.h"
#include "../assets/orange_brick_xpm.h"
#include "../assets/red_brick_xpm.h"
#include "../assets/yellow_brick_xpm.h"
#include "../assets/x2powerup_xpm.h"
#include "../assets/numbers/number_zero_xpm.h"
#include "../assets/numbers/number_one_xpm.h"
#include "../assets/numbers/number_two_xpm.h"
#include "../assets/numbers/number_three_xpm.h"
#include "../assets/numbers/number_four_xpm.h"
#include "../assets/numbers/number_five_xpm.h"
#include "../assets/numbers/number_six_xpm.h"
#include "../assets/numbers/number_seven_xpm.h"
#include "../assets/numbers/number_eight_xpm.h"
#include "../assets/numbers/number_nine_xpm.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

  breakout->ball = create_sprite((xpm_map_t) ball_xpm);

  breakout->lives = 3;

  breakout->score = 0;

  create_bricks(breakout);

  if (breakout->ball == NULL) {
    destroy_sprite(breakout->ball);
    free(breakout);
    return NULL;
  }

  breakout->ball->x = 395;
  breakout->ball->y = 485;
  breakout->ball->xspeed = 0;
  breakout->ball->yspeed = 0;
  breakout->ball_attached = true;

  // Initialize powerups
  for (int i = 0; i < 10; i++) {
    breakout->powerups[i] = NULL;
    breakout->active_powerups[i] = false;
  }

  return breakout;
}

void create_bricks(breakout_t *breakout) {
  for (int i = 0; i < 12; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) red_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
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
      return;
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
      return;
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
      return;
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
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 48) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 220;
  }

  for (int i = 0; i < 60; i++) {
    breakout->active_bricks[i] = true;
  }
}

int draw_bricks(breakout_t *breakout) {
  for (int i = 0; i < 60; i++) {
    if (breakout->active_bricks[i] && draw_sprite(breakout->bricks[i], breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
      return 1;
    }
  }
  return 0;
}

int draw_lives(breakout_t *breakout) {
  for (int i = 0; i < breakout->lives; i++) {
    if (draw_xpm((xpm_map_t) ball_xpm, (i * 15) + 5, 5) != 0) {
      return 1;
    }
  }
  return 0;
}

int draw_score(breakout_t *breakout) {
  const xpm_map_t digit_xpms[] = {
    (xpm_map_t) number_zero_xpm, (xpm_map_t) number_one_xpm, (xpm_map_t) number_two_xpm, 
    (xpm_map_t) number_three_xpm, (xpm_map_t) number_four_xpm, (xpm_map_t) number_five_xpm, 
    (xpm_map_t) number_six_xpm, (xpm_map_t) number_seven_xpm, (xpm_map_t) number_eight_xpm, 
    (xpm_map_t) number_nine_xpm
  };

  if (breakout->score == 0) {
    if (draw_xpm(digit_xpms[0], 395, 5)) {
      return 1;
    }
    return 0;
  }

  char score_str[20];
  sprintf(score_str, "%d", breakout->score);
  int num_digits = strlen(score_str);
  
  int digit_width = 10;
  int total_width = num_digits * digit_width;
  int start_x = 400 - (total_width / 2);

  for (int i = 0; i < num_digits; i++) {
    int digit = score_str[i] - '0';
    if (draw_xpm(digit_xpms[digit], start_x + (i * digit_width), 5)) {
      return 1;
    }
  }
  
  return 0;
}

int draw_powerup(breakout_t *breakout){
  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      if (draw_sprite(breakout->powerups[i], breakout->powerups[i]->x, breakout->powerups[i]->y) != 0) {
        return 1;
      }
    }
  }
  return 0;
}

void spawn_powerup(breakout_t *breakout, int x, int y) {
  if (breakout == NULL) return;
  
  // Find an empty slot for the powerup
  for (int i = 0; i < 10; i++) {
    if (!breakout->active_powerups[i]) {
      breakout->powerups[i] = create_sprite((xpm_map_t) x2powerup_xpm);
      if (breakout->powerups[i] != NULL) {
        // Set position - randomize x position if not specified
        if (x < 0) {
          // Random x position between 50 and 750 (to keep within screen bounds)
          breakout->powerups[i]->x = 50 + (rand() % 701);
        } else {
          breakout->powerups[i]->x = x;
        }
        
        if (y < 0) {
          // Random y position between bricks (220) and bar (500)
          breakout->powerups[i]->y = 250 + (rand() % 200);
        } else {
          breakout->powerups[i]->y = y;
        }
        
        // Set downward movement speed
        breakout->powerups[i]->yspeed = 2;
        breakout->active_powerups[i] = true;
        break;
      }
    }
  }
}

void update_powerups(breakout_t *breakout) {
  if (breakout == NULL) return;
  
  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      // Move powerup down
      breakout->powerups[i]->y += breakout->powerups[i]->yspeed;
      
      // Remove powerup if it goes off screen (below bar area)
      if (breakout->powerups[i]->y > 550) {
        destroy_sprite(breakout->powerups[i]);
        breakout->powerups[i] = NULL;
        breakout->active_powerups[i] = false;
      }
    }
  }
}

int draw_breakout(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return 1;
  }

  if (draw_sprite(breakout->bar, breakout->bar->x, breakout->bar->y) != 0) {
    return 1;
  }

  draw_bricks(breakout);

  draw_lives(breakout);

  draw_score(breakout);

  // Update and draw powerups
  update_powerups(breakout);
  handle_powerup_collisions(breakout);
  draw_powerup(breakout);

  if (draw_sprite(breakout->ball, breakout->ball->x, breakout->ball->y) != 0) {
    return 1;
  }

  int count = 0;

  for (int i = 0; i < 60; i++) {
    if (breakout->active_bricks[i] == false) {
      count++;
    }
  }

  if (count == 60) {
    create_bricks(breakout);
    draw_bricks(breakout);

    breakout->ball->x = 395;
    breakout->ball->y = 485;
    breakout->ball->xspeed = 3;
    breakout->ball->yspeed = -3;
  }

  return 0;
}

void handle_powerup_collisions(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL) return;
  
  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      // Check collision between powerup and bar
      if (breakout->powerups[i]->y + breakout->powerups[i]->height >= breakout->bar->y &&
          breakout->powerups[i]->y <= breakout->bar->y + breakout->bar->height &&
          breakout->powerups[i]->x + breakout->powerups[i]->width >= breakout->bar->x &&
          breakout->powerups[i]->x <= breakout->bar->x + breakout->bar->width) {
        
        // Powerup collected! Apply effect (for now, just add score)
        breakout->score += 500; // Bonus points for collecting powerup
        
        // TODO: Add specific powerup effects here (e.g., x2 multiplier, extra life, etc.)
        
        // Remove the powerup
        destroy_sprite(breakout->powerups[i]);
        breakout->powerups[i] = NULL;
        breakout->active_powerups[i] = false;
      }
    }
  }
}

void destroy_breakout(breakout_t *breakout) {
  if (breakout == NULL) {
    return;
  }

  if (breakout->bar != NULL) {
    destroy_sprite(breakout->bar);
  }

  for (int i = 0; i < 60; i++) {
    if (breakout->bricks[i] != NULL) {
      destroy_sprite(breakout->bricks[i]);
    }
  }

  // Clean up powerups
  for (int i = 0; i < 10; i++) {
    if (breakout->powerups[i] != NULL) {
      destroy_sprite(breakout->powerups[i]);
    }
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
    breakout->lives--;
    breakout->ball->x = breakout->bar->x + (breakout->bar->width / 2) - (breakout->ball->width / 2);
    breakout->ball->y = 485;
    breakout->ball->xspeed = 0;
    breakout->ball->yspeed = 0;
    breakout->ball_attached = true;
  }

  if (breakout->ball->y + breakout->ball->height >= breakout->bar->y &&
      breakout->ball->y <= breakout->bar->y + breakout->bar->height &&
      breakout->ball->x + breakout->ball->width >= breakout->bar->x &&
      breakout->ball->x <= breakout->bar->x + breakout->bar->width) {
    
    int ball_center_x = breakout->ball->x + breakout->ball->width / 2;
    int bar_center_x = breakout->bar->x + breakout->bar->width / 2;
    
    float relative_hit = (float)(ball_center_x - bar_center_x) / (breakout->bar->width / 2);
    
    if (relative_hit < -1.0f) relative_hit = -1.0f;
    if (relative_hit > 1.0f) relative_hit = 1.0f;
    
    float current_speed = sqrt(breakout->ball->xspeed * breakout->ball->xspeed + 
                              breakout->ball->yspeed * breakout->ball->yspeed);
    
    float angle = relative_hit * (M_PI / 3.0f);
    
    breakout->ball->xspeed = (int)(current_speed * sin(angle));
    breakout->ball->yspeed = -(int)(current_speed * cos(angle));
    
    if (breakout->ball->yspeed > -3) {
      breakout->ball->yspeed = -3;
    }
  }

  for (int i = 0; i < 60; i++) {
    if (!breakout->active_bricks[i])
      continue;

    if (breakout->ball->y <= breakout->bricks[i]->y + breakout->bricks[i]->height &&
        breakout->ball->y + breakout->ball->height >= breakout->bricks[i]->y &&
        breakout->ball->x + breakout->ball->width >= breakout->bricks[i]->x &&
        breakout->ball->x <= breakout->bricks[i]->x + breakout->bricks[i]->width) {
      if (breakout->ball->y + breakout->ball->height - breakout->ball->yspeed <= breakout->bricks[i]->y ||
          breakout->ball->y - breakout->ball->yspeed >= breakout->bricks[i]->y + breakout->bricks[i]->height) {
        breakout->ball->yspeed = -breakout->ball->yspeed;
      }
      else {
        breakout->ball->xspeed = -breakout->ball->xspeed;
      }
      breakout->score += 100;
      breakout->active_bricks[i] = false;
      
      // 20% chance to spawn a powerup when a brick is destroyed
      if ((rand() % 100) < 20) {
        spawn_powerup(breakout, breakout->bricks[i]->x + breakout->bricks[i]->width / 2, 
                     breakout->bricks[i]->y + breakout->bricks[i]->height);
      }
      
      break;
    }
  }
}

void move_bar_with_ball_keyboard(breakout_t *breakout, int direction) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return;
  }

  int old_bar_x = breakout->bar->x;
  
  if (direction < 0) {
    move_sprite_left(breakout->bar);
  } else if (direction > 0) {
    move_sprite_right(breakout->bar);
  }

  if (breakout->ball_attached) {
    int bar_movement = breakout->bar->x - old_bar_x;
    breakout->ball->x += bar_movement;
  }
}

void move_bar_with_ball_mouse(breakout_t *breakout, int target_x, int target_y, int xspeed) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return;
  }

  int old_bar_x = breakout->bar->x;
  
  move_sprite_mouse(breakout->bar, target_x, target_y, xspeed);

  if (breakout->ball_attached) {
    int bar_movement = breakout->bar->x - old_bar_x;
    breakout->ball->x += bar_movement;
  }
}
