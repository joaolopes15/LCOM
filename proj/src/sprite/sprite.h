#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"

typedef struct {
  int x, y;           // current position
  int width, height;  // dimensions
  int xspeed, yspeed; // current speed
  uint32_t *map;          // the pixmap
} Sprite;

Sprite *create_sprite(xpm_map_t xpm);

void destroy_sprite(Sprite *sprite);

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);

#endif
