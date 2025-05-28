#ifndef INSTRUCTIONMENU_H
#define INSTRUCTIONMENU_H

#include "../game/game.h"
#include "../sprite/sprite.h"

void instructionmenu_process_input(game_t *game, uint8_t scancode);
void instructionmenu_render(game_t *game);

#endif
