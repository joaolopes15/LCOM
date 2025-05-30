#include "game.h"
#include "../assets/bar_xpm.h"
#include "../assets/logo_xpm.h"
#include "../assets/test_xpm.h"
#include "../menus/gameovermenu.h"
#include "../menus/instructionmenu.h"
#include "../menus/mainmenu.h"
#include "../menus/pausemenu.h"
#include "../menus/menu_action_enum.h"
#include <stdlib.h>

// TODO: put all scancodes in a separate header file
// initialize the game structure and set initial state
game_t *game_init() {
  game_t *game = (game_t *) malloc(sizeof(game_t));

  if (game == NULL) {
    printf("Error allocating memory for game structure\n");
    return NULL;
  }

  game->main_menu = main_menu_init();
  game->current_state = STATE_MENU;

  game->key_left_pressed = false;
  game->key_right_pressed = false;
  game->key_up_pressed = false;
  game->key_down_pressed = false;
  game->key_space_pressed = false;

  game->pause_menu_selected_option = 0;
  game->game_over_selected_option = 0;

  game->mouse_x = 400;
  game->mouse_y = 300;
  game->mouse_target_x = 350;
  game->mouse_control_active = false;

  game->breakout = NULL;

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
      else if (key_code == 0x48) { // up arrow
        game->key_up_pressed = !is_release;
      }
      else if (key_code == 0x50) { // down arrow
        game->key_down_pressed = !is_release;
      }
      else if (key_code == 0x39) {
        game->key_space_pressed = !is_release;
      }
      break;
    case STATE_MENU: {
      menu_action_t action = mainmenu_process_input(game->main_menu, scancode);
      switch (action) {
        case MENU_ACTION_START_GAME:
          game_change_state(game, STATE_PLAYING);
          break;
        case MENU_ACTION_HOW_TO_PLAY:
          game_change_state(game, STATE_HOW_TO_PLAY);
          break;
        case MENU_ACTION_EXIT:
          game_change_state(game, STATE_EXIT);
          break;
        case MENU_ACTION_NONE:
        default:
          break;
      }
      break;
    }
    case STATE_PAUSED:
      pausemenu_process_input(game, scancode);
      break;
    case STATE_GAME_OVER:
      gameovermenu_process_input(game, scancode);
      break;
    case STATE_HOW_TO_PLAY: {
      menu_action_t action = instructionmenu_process_input(game->instruction_menu, scancode);
      switch (action) {
        case MENU_ACTION_MAIN_MENU:
          game_change_state(game, STATE_MENU);
          break;
        case MENU_ACTION_NONE:
        default:
          break;
      }
    } break;

    case STATE_EXIT:
      break;
  }
}

// process mouse input according to the current game state
void game_process_mouse_input(game_t *game, struct packet *mouse_packet) {
  if (game == NULL || mouse_packet == NULL)
    return;

  game->mouse_x += mouse_packet->delta_x;
  game->mouse_y += mouse_packet->delta_y;

  if (game->mouse_x < 0)
    game->mouse_x = 0;
  if (game->mouse_x > vmi_p.XResolution)
    game->mouse_x = vmi_p.XResolution;
  if (game->mouse_y < 0)
    game->mouse_y = 0;
  if (game->mouse_y > vmi_p.YResolution)
    game->mouse_y = vmi_p.YResolution;

  switch (game->current_state) {
    case STATE_PLAYING:
      if (game->breakout != NULL && game->breakout->bar != NULL) {
        game->mouse_target_x = game->mouse_x - game->breakout->bar->width / 2;

        if (game->mouse_target_x < 0)
          game->mouse_target_x = 0;
        if (game->mouse_target_x + game->breakout->bar->width > vmi_p.XResolution)
          game->mouse_target_x = vmi_p.XResolution - game->breakout->bar->width;

        game->mouse_control_active = true;
      }
      break;

    case STATE_MENU:
    case STATE_PAUSED:
    case STATE_GAME_OVER:
    case STATE_EXIT:
    case STATE_HOW_TO_PLAY:
      game->mouse_control_active = false;
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
      if (game->breakout != NULL) {
        if (game->key_left_pressed) {
          move_bar_with_ball_keyboard(game->breakout, -1);
          game->mouse_control_active = false;
        }
        else if (game->key_right_pressed) {
          move_bar_with_ball_keyboard(game->breakout, 1);
          game->mouse_control_active = false;
        }
        else if (game->mouse_control_active && game->breakout->bar != NULL) {
          move_bar_with_ball_mouse(game->breakout, game->mouse_target_x, game->breakout->bar->y, 100);
        }
        if (game->key_space_pressed) {
          game->breakout->ball->xspeed = 0;
          game->breakout->ball->yspeed = -3;
          game->breakout->ball_attached = false;
        }
        if (game->breakout->lives == 0) {
          game_change_state(game, STATE_GAME_OVER);
        }
        if (!game->breakout->ball_attached) {
          game->breakout->ball->x += game->breakout->ball->xspeed;
          game->breakout->ball->y += game->breakout->ball->yspeed;
        }
        handle_ball_collisions(game->breakout);
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
      clear_screen();
      draw_main_menu(game->main_menu);
      break;
    case STATE_PAUSED:
      pausemenu_render(game);
      break;
    case STATE_GAME_OVER:
      gameovermenu_render(game);
      break;
    case STATE_HOW_TO_PLAY:
      clear_screen();
      draw_instruction_menu(game->instruction_menu);
      break;
    case STATE_PLAYING:
      clear_screen();
      draw_breakout(game->breakout);
      break;
    case STATE_EXIT:
      break;
  }
}

// change the current game state
void game_change_state(game_t *game, game_state_t new_state) {
  if (game == NULL)
    return;

  game->current_state = new_state;

  switch (new_state) {
    case STATE_MENU:
      if (game->breakout != NULL) {
        destroy_breakout(game->breakout);
        game->breakout = NULL;
      }
      break;

    case STATE_PLAYING:
      if (game->breakout == NULL) {
        game->breakout = breakout_init();
        if (game->breakout == NULL) {
          printf("Error initializing breakout game\n");
          game_change_state(game, STATE_EXIT);
        }
      }
      if (game->main_menu != NULL) {
        destroy_main_menu(game->main_menu);
        game->main_menu = NULL;
      }
      break;

    case STATE_PAUSED:
      break;

    case STATE_GAME_OVER:
      break;

    case STATE_HOW_TO_PLAY:
      if (game->instruction_menu == NULL) {
        game->instruction_menu = instruction_menu_init();
        if (game->instruction_menu == NULL) {
          printf("Error initializing instruction menu\n");
          game_change_state(game, STATE_EXIT);
        }
      }
      break;

    case STATE_EXIT:
      break;
  }
}

// clean up game resources before exiting
void game_exit(game_t *game) {
  if (game == NULL)
    return;

  free(game);
}
