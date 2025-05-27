#include "game.h"
#include "../assets/bar_xpm.h"
#include "../assets/test_xpm.h"
#include "../assets/logo_xpm.h"
#include "../assets/menus/welcome_xpm.h" 
#include "../assets/menus/exit_xpm.h"
#include "../assets/menus/startS_xpm.h"
#include "../assets/menus/startgame_xpm.h"
#include "../assets/menus/howtoplayS_xpm.h"
#include "../assets/menus/howtoplay_xpm.h"
#include "../assets/menus/to_xpm.h"
#include "../assets/menus/paused_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/continueS_xpm.h"
#include "../assets/menus/continue_xpm.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/retry_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/gameover_xpm.h"
#include "../assets/menus/highscore_xpm.h"
#include "../sprite/sprite.h"
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
    case STATE_MENU:
      // Handle non-movement keys (only on press, not release)
      if (!is_release) {
        if (scancode == 0x1C) { // enter key
          if (game->main_menu_selected_option == 0) { // Start Game
            game_change_state(game, STATE_PLAYING);
          } else if (game->main_menu_selected_option == 2) { // Exit
            game_change_state(game, STATE_EXIT);
          }
          // Option 1 (How to Play) can be handled here if needed
        }
        else if (scancode == 0x01) { // esc key to exit
          game_change_state(game, STATE_EXIT);
        }
        else if (key_code == 0x48) { // up arrow
          if (game->main_menu_selected_option > 0) game->main_menu_selected_option--;
        }
        else if (key_code == 0x50) { // down arrow
          if (game->main_menu_selected_option < 2) game->main_menu_selected_option++;
        }
      }
      break;

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

    case STATE_PAUSED:
      // Handle non-movement keys (only on press, not release)
      if (!is_release) {
        if (scancode == 0x1C) { // enter key
          if (game->pause_menu_selected_option == 0) { // Continue
            game_change_state(game, STATE_PLAYING);
          }
          else if (game->pause_menu_selected_option == 1) { // Retry
            game_change_state(game, STATE_PLAYING);
            // Reset game state if needed, e.g., reset score, lives, etc.
            game->barra->x = 350; // Reset barra position
            game->barra->y = 500; // Reset barra position
          }
          else if (game->pause_menu_selected_option == 2) { // Exit to menu
            game_change_state(game, STATE_MENU);
          }
        }
        else if (key_code == 0x48) { // up arrow
          if (game->pause_menu_selected_option > 0) game->pause_menu_selected_option--;
        }
        else if (key_code == 0x50) { // down arrow
          if (game->pause_menu_selected_option < 2) game->pause_menu_selected_option++;
        }
      }
      break;

    case STATE_GAME_OVER:
      if (!is_release) {
        if (scancode == 0x1C) { // enter key
          if (game->game_over_selected_option == 0) { // Retry
            game_change_state(game, STATE_PLAYING);
            // Reset game state if needed, e.g., reset score, lives, etc.
            game->barra->x = 350; // Reset barra position
            game->barra->y = 500; // Reset barra position;
          }
          else if (game->game_over_selected_option == 1) { // Exit to menu
            game_change_state(game, STATE_MENU);  

          }
          else if (game->game_over_selected_option == 2) { // Exit
            game_change_state(game, STATE_EXIT);
          }
        }
        else if (key_code == 0x48) { // up arrow
          if (game->game_over_selected_option > 0) game->game_over_selected_option--;
        }
        else if (key_code == 0x50) { // down arrow
          if (game->game_over_selected_option < 2) game->game_over_selected_option++;
        }
      }
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
      clear_screen();
      draw_xpm((xpm_map_t) welcome_xpm , 200, 0);
      draw_xpm((xpm_map_t) to_xpm, 450, 0);
      draw_xpm((xpm_map_t) logo_xpm, 270, 100);
      // Draw menu options, highlight selected
      if (game->main_menu_selected_option == 0)
        draw_xpm((xpm_map_t) startS_xpm, 250, 300);
      else
        draw_xpm((xpm_map_t) startgame_xpm, 250, 300);
      if (game->main_menu_selected_option == 1)
        draw_xpm((xpm_map_t) howtoplayS_xpm, 250, 400);
      else
        draw_xpm((xpm_map_t) howtoplay_xpm, 250, 400);
      if (game->main_menu_selected_option == 2)
        draw_xpm((xpm_map_t) exitS_xpm, 230, 500);
      else
        draw_xpm((xpm_map_t) exit_xpm, 250, 500);
      break;

    case STATE_PLAYING:
      clear_screen();
      draw_sprite(game->barra, game->barra->x, game->barra->y);
      break;

    case STATE_PAUSED:
      clear_screen();
      draw_xpm((xpm_map_t) paused_xpm, 250, 100);
      if(game->pause_menu_selected_option == 0)
        draw_xpm((xpm_map_t) continueS_xpm, 250, 200);
      else{
        draw_xpm((xpm_map_t) continue_xpm, 250, 200);
      }
      if(game->pause_menu_selected_option == 1)
        draw_xpm((xpm_map_t) retryS_xpm, 250, 300);
      else{
        draw_xpm((xpm_map_t) retry_xpm, 250, 300);
      }
      if(game->pause_menu_selected_option == 2)
        draw_xpm((xpm_map_t) exittomenuS_xpm, 250, 400);
      else{
        draw_xpm((xpm_map_t) exittomenu_xpm, 250, 400);
      }
      break;

    case STATE_GAME_OVER:
      clear_screen();
      draw_xpm((xpm_map_t) gameover_xpm, 250, 100);
      draw_xpm((xpm_map_t) highscore_xpm, 250, 200);
      if (game->game_over_selected_option == 0)
        draw_xpm((xpm_map_t) retryS_xpm, 250, 300);
      else
        draw_xpm((xpm_map_t) retry_xpm, 250, 300);
      if (game->game_over_selected_option == 1)
        draw_xpm((xpm_map_t) exittomenuS_xpm, 250, 400);
      else
        draw_xpm((xpm_map_t) exittomenu_xpm, 250, 400);
      if(game->game_over_selected_option == 2)
        draw_xpm((xpm_map_t) exitS_xpm, 250, 500);
      else
        draw_xpm((xpm_map_t) exit_xpm, 250, 500);

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
