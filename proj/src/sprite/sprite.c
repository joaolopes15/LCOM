#include "sprite.h"

Sprite *create_sprite(xpm_map_t xpm) {
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));

    if (sprite == NULL) {
        return NULL;
    }

    xpm_image_t img;

    sprite->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    if (sprite->map == NULL) {
        free(sprite);
        return NULL;
    }

    sprite->width = img.width;
    sprite->height = img.height;

    return sprite;
}

void destroy_sprite(Sprite *sprite) {
    if (sprite != NULL) {
        if (sprite->map != NULL) {
            free(sprite->map);
        }
        free(sprite);
    }
    sprite = NULL;
}

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y) {
  for (int i = 0; i < sprite->height; i++) {
    for (int j = 0; j < sprite->width; j++) {
      unsigned pixel_index = i * sprite->width + j;
      uint32_t color = sprite->map[pixel_index];

      if (color != xpm_transparency_color(XPM_8_8_8_8)) {
        if (vg_draw_pixel(x + j, y + i, color) != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}
