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
