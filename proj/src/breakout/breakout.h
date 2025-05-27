#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"

typedef struct {
  Sprite *bar;
} breakout_t;

breakout_t* breakout_init();

#endif
