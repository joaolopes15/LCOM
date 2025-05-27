#include "breakout.h"
#include "../assets/bar_xpm.h"

breakout_t *breakout_init() {
  breakout_t *breakout = (breakout_t *)malloc(sizeof(breakout_t));

  if (breakout == NULL) {
    return NULL;
  }

  breakout->bar = create_sprite((xpm_map_t) bar_xpm);
  if (breakout->bar == NULL) {
    free(breakout);
    return NULL;
  }

  breakout->bar->x = 350;
  breakout->bar->y = 500;

  return breakout;
}

int draw_breakout(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL) {
    return 1;
  }

  if (draw_sprite(breakout->bar, breakout->bar->x, breakout->bar->y) != 0) {
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

  free(breakout);
}
