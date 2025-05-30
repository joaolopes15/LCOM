#ifndef MAINMENU_H
#define MAINMENU_H

#include "../sprite/sprite.h"
#include "background.h"

typedef enum {
  MENU_ACTION_NONE,
  MENU_ACTION_START_GAME,
  MENU_ACTION_HOW_TO_PLAY,
  MENU_ACTION_EXIT
} menu_action_t;

typedef struct {
  int selected_option;

  Sprite *welcome_sprite;
  Sprite *logo_sprite;
  Sprite *startS_sprite;
  Sprite *start_sprite;
  Sprite *howtoplayS_sprite;
  Sprite *howtoplay_sprite;
  Sprite *exitS_sprite;
  Sprite *exit_sprite;
} main_menu_t;

main_menu_t* main_menu_init();

menu_action_t mainmenu_process_input(main_menu_t *main_menu, uint8_t scancode);

void draw_main_menu(main_menu_t *main_menu);

void destroy_main_menu(main_menu_t *main_menu);

#endif
