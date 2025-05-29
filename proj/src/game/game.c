#include "game.h"
#include "../assets/bar_xpm.h"
#include "../assets/test_xpm.h"
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
  game->key_up_pressed = false;
  game->key_down_pressed = false;
  game->key_space_pressed = false;

  
  game->mouse_x = 400; 
  game->mouse_y = 300;
  game->mouse_target_x = 350; 
  game->mouse_control_active = false;

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
        if (scancode == 0x1C) { // enter key to start game
          game_change_state(game, STATE_PLAYING);
        }
        else if (scancode == 0x01) { // esc key to exit
          game_change_state(game, STATE_EXIT);
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
      break;

    case STATE_PAUSED:
      if (scancode == 0x01) { // esc key to resume
        game_change_state(game, STATE_PLAYING);
      }
      else if (scancode == 0x10) { // Q key to quit to menu
        game_change_state(game, STATE_MENU);
      }
      break;

    case STATE_GAME_OVER:
      if (scancode == 0x1C) { // enter key to return to menu
        game_change_state(game, STATE_MENU);
      }
      else if (scancode == 0x01) { // esc key to exit
        game_change_state(game, STATE_EXIT);
      }
      break;

    case STATE_EXIT:
      break;
  }
}

// process mouse input according to the current game state
void game_process_mouse_input(game_t *game, struct packet *mouse_packet) {
  if (game == NULL || mouse_packet == NULL)
    return;

  // Update mouse position
  game->mouse_x += mouse_packet->delta_x;
  game->mouse_y += mouse_packet->delta_y;

  // Clamp mouse position to screen bounds
  if (game->mouse_x < 0) game->mouse_x = 0;
  if (game->mouse_x > vmi_p.XResolution) game->mouse_x = vmi_p.XResolution;
  if (game->mouse_y < 0) game->mouse_y = 0;
  if (game->mouse_y > vmi_p.YResolution) game->mouse_y = vmi_p.YResolution;

  switch (game->current_state) {
    case STATE_PLAYING:
      // Set target position for bar sprite to follow mouse X position
      // Center the bar on the mouse X position
      game->mouse_target_x = game->mouse_x - game->barra->width / 2;
      
      // Clamp target position to screen bounds
      if (game->mouse_target_x < 0) 
        game->mouse_target_x = 0;
      if (game->mouse_target_x + game->barra->width > vmi_p.XResolution) 
        game->mouse_target_x = vmi_p.XResolution - game->barra->width;
      
      game->mouse_control_active = true;
      break;
    
    case STATE_MENU:
    case STATE_PAUSED:
    case STATE_GAME_OVER:
    case STATE_EXIT:
      // Mouse input ignored in these states
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
      if (game->key_left_pressed) {
        move_sprite_left(game->barra);
        move_bar_with_ball(game->breakout, -1);
        game->mouse_control_active = false; // Disable mouse control when using keyboard
      }
      else if (game->key_right_pressed) {
        move_sprite_right(game->barra);
        move_bar_with_ball(game->breakout, 1);
        game->mouse_control_active = false; // Disable mouse control when using keyboard
      }
      else if (game->mouse_control_active) {
        // Only move towards mouse target if no keyboard input is active
        move_sprite_mouse(game->barra, game->mouse_target_x, game->barra->y, 100);
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
      vg_draw_rectangle(250, 250, 100, 100, 0x8e3ba6);
      break;

    case STATE_PLAYING:
      clear_screen();
      draw_sprite(game->barra, game->barra->x, game->barra->y);
      break;

    case STATE_PAUSED:
      clear_screen();
      vg_draw_rectangle(150, 150, 300, 200, 0xffffff);
      break;

    case STATE_GAME_OVER:
      clear_screen();
      vg_draw_rectangle(100, 100, 400, 300, 0x3);
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
