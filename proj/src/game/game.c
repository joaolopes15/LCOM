#include "game.h"
#include "../assets/bar_xpm.h"
#include "../assets/test_xpm.h"
#include "../assets/logo_xpm.h"
#include "../sprite/sprite.h"
#include "../menus/mainmenu.h"
#include "../menus/pausemenu.h"
#include "../menus/gameovermenu.h"
#include "../menus/instructionmenu.h"
#include <stdlib.h>

// initialize the game structure and set initial state
game_t *game_init() {
  game_t *game = (game_t *) malloc(sizeof(game_t));

  if (game == NULL) {
    printf("Error allocating memory for game structure\n");
    return NULL;
  }

  game->current_state = STATE_MENU;
  game->previous_state = STATE_MENU;
  game->barra = create_sprite((xpm_map_t) bar_xpm);

  game->key_left_pressed = false;
  game->key_right_pressed = false;

  // Initialize menu selection
  game->main_menu_selected_option = 0;
  game->pause_menu_selected_option = 0;
  game->game_over_selected_option = 0;

  if (game->barra == NULL) {
    printf("Error creating barra sprite\n");
    free(game);
    return NULL;
  }

  game->barra->x = 350;
  game->barra->y = 500;

  return game;
}

// process keyboard input according to the current game state
void game_process_input(game_t *game, uint8_t scancode) {
  if (game == NULL || scancode == 0)
    return;

  bool is_release = (scancode & 0x80) != 0;
  uint8_t key_code = scancode & 0x7F;

  switch (game->current_state) {

    case STATE_PLAYING:
      if (scancode == 0x01) { // esc key to pause
        game_change_state(game, STATE_PAUSED);
      }
      if (key_code == 0x4B) { // left arrow
        game->key_left_pressed = !is_release;
      }
      else if (key_code == 0x4D) { // right arrow
        game->key_right_pressed = !is_release;
      }
      else if (key_code == 0x10 && !is_release) { // 'q' key pressed change this later
        game_change_state(game, STATE_GAME_OVER);
      }
      break;
        case STATE_MENU:
            mainmenu_process_input(game, scancode);
            break;
        case STATE_PAUSED:
            pausemenu_process_input(game, scancode);
            break;
        case STATE_GAME_OVER:
            gameovermenu_process_input(game, scancode);
            break;
        case STATE_HOW_TO_PLAY:
            instructionmenu_process_input(game, scancode);
            break;

    case STATE_EXIT:
      break;
  }
}

// update the game logic based on the current state
void game_update(game_t *game) {
  if (game == NULL)
    return;

  switch (game->current_state) {
    case STATE_MENU:
      break;

    case STATE_PLAYING:
      if (game->key_left_pressed) {
        move_sprite_left(game->barra);
      }
      if (game->key_right_pressed) {
        move_sprite_right(game->barra);
      }
      break;

    case STATE_PAUSED:
      break;

    case STATE_GAME_OVER:
      break;

    case STATE_HOW_TO_PLAY:
      break;

    case STATE_EXIT:
      break;
  }
}

// render the game according to the current state
void game_render(game_t *game) {
  if (game == NULL)
    return;

  switch (game->current_state) {
            case STATE_MENU:
            mainmenu_render(game);
            break;
        case STATE_PAUSED:
            pausemenu_render(game);
            break;
        case STATE_GAME_OVER:
            gameovermenu_render(game);
            break;
        case STATE_HOW_TO_PLAY:
            instructionmenu_render(game);
            break;

    case STATE_PLAYING:
      clear_screen();
      draw_sprite(game->barra, game->barra->x, game->barra->y);
      break;
    case STATE_EXIT:
      break;
  }
}

// change the current game state
void game_change_state(game_t *game, game_state_t new_state) {
  if (game == NULL)
    return;

  game->previous_state = game->current_state;

  game->current_state = new_state;

  switch (new_state) {
    case STATE_MENU:
      break;

    case STATE_PLAYING:
      break;

    case STATE_PAUSED:
      break;

    case STATE_GAME_OVER:
      break;

    case STATE_HOW_TO_PLAY:
      break;

    case STATE_EXIT:
      break;
  }
}

// clean up game resources before exiting
void game_exit(game_t *game) {
  if (game == NULL)
    return;

  if (game->barra != NULL) {
    destroy_sprite(game->barra);
  }

  free(game);
}

