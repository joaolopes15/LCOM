#ifndef INSTRUCTIONMENU_H
#define INSTRUCTIONMENU_H

#include "../sprite/sprite.h"
#include "menu_action_enum.h"

typedef struct {
  Sprite *instruction_sprite;
  Sprite *gameplay_instruction_sprite;
} instruction_menu_t;

instruction_menu_t* instruction_menu_init();

menu_action_t instructionmenu_process_input(instruction_menu_t *instruction_menu, uint8_t scancode);
void draw_instruction_menu(instruction_menu_t *instruction_menu);
void destroy_instruction_menu(instruction_menu_t *intruction_menu);

#endif
