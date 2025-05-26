#include "game.h"
#include "../assets/test_xpm.h"
#include "../assets/logo_xpm.h"
#include <stdlib.h>

// initialize the game structure and set initial state
game_t* game_init() {
  game_t* game = (game_t*)malloc(sizeof(game_t));
  
  if (game == NULL) {
    printf("Error allocating memory for game structure\n");
    return NULL;
  }
  
  game->current_state = STATE_MENU;
  game->previous_state = STATE_MENU;
  
  return game;
}

// process keyboard input according to the current game state
void game_process_input(game_t* game, uint8_t scancode) {
  if (game == NULL) return;
  
  switch (game->current_state) {
    case STATE_MENU:
      if (scancode == 0x1C) { // enter key to start game
        game_change_state(game, STATE_PLAYING);
      }
      else if (scancode == 0x01) { // esc key to exit
        game_change_state(game, STATE_EXIT);
      }
      break;
      
    case STATE_PLAYING:
      if (scancode == 0x01) { // esc key to pause
        game_change_state(game, STATE_PAUSED);
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

// update the game logic based on the current state
void game_update(game_t* game) {
  if (game == NULL) return;
  
  switch (game->current_state) {
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

// render the game according to the current state
void game_render(game_t* game) {
  if (game == NULL) return;
  
  clear_screen();
  
  switch (game->current_state) {
    case STATE_MENU:
      draw_xpm((xpm_map_t)logo_xpm, 0, 0);
      break;
      
    case STATE_PLAYING:
      vg_draw_rectangle(250, 250, 100, 100, 0x8e3ba6);
      break;
      
    case STATE_PAUSED:
      vg_draw_rectangle(150, 150, 300, 200, 0xffffff);
      break;
      
    case STATE_GAME_OVER:
      vg_draw_rectangle(100, 100, 400, 300, 0x3);
      break;
      
    case STATE_EXIT:
      break;
  }
}

// change the current game state
void game_change_state(game_t* game, game_state_t new_state) {
  if (game == NULL) return;
  
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
void game_exit(game_t* game) {
  if (game == NULL) return;
  
  free(game);
}
