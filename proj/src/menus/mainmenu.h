#ifndef MAINMENU_H
#define MAINMENU_H

#include "../game/game.h"
#include "../sprite/sprite.h"
#include "background.h"

void mainmenu_process_input(game_t *game, uint8_t scancode);
void mainmenu_render(game_t *game);

#endif
