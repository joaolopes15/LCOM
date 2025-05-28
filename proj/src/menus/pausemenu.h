#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "../game/game.h"
#include "../sprite/sprite.h"

void pausemenu_process_input(game_t *game, uint8_t scancode);
void pausemenu_render(game_t *game);

#endif
