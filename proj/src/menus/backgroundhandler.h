#ifndef BACKGROUNDHANDLER_H
#define BACKGROUNDHANDLER_H

#include "../sprite/sprite.h"

void draw_sprite_to_memory(Sprite *sprite, int x, int y, uint8_t *video_mem, uint16_t screen_width, uint16_t screen_height);

void draw_background_to_memory(uint8_t *video_mem, uint16_t screen_width, uint16_t screen_height);

#endif