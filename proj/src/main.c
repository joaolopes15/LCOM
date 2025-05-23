#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "drivers/keyboard/keyboard.h"
#include "drivers/video/video.h"
#include "game/game.h"

extern uint8_t scancode;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  message msg;
  uint8_t irq_set_keyboard, irq_set_timer;
  game_t* game;

  // Initialize the game
  game = game_init();
  if (game == NULL) {
    printf("Failed to initialize game\n");
    return 1;
  }

  if (timer_set_frequency(0, 60) != 0) {
    game_exit(game);
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    game_exit(game);
    return 1;
  }

  if (keyboard_subscribe_int(&irq_set_keyboard) != 0) {
    timer_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  if (vg_init(0x115) == NULL) {
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    game_exit(game);
    return 1;
  }

  // Main game loop
  while (game->current_state != STATE_EXIT) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            kbc_ih();
            
            // Print scancode for debugging
            printf("Scancode: 0x%02x\n", scancode);
            
            // Process input with the received scancode
            game_process_input(game, scancode);
            
          }
          
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            
            // Debug print once per second
            if (counter % 60 == 0) {
              printf("State: %d\n", game->current_state);
            }
            
            // Update and render game at each timer tick
            game_update(game);
            game_render(game);
            
            // Flip the buffers to show the rendered frame
            flip();
          }
          break;
          
        default:
          break;
      }
    } else {
      // Handle other messages
    }
  }

  // Clean up resources
  if (vg_exit() != 0) {
    printf("Failed to exit video mode\n");
  }

  if (timer_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe timer interrupts\n");
  }

  if (keyboard_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe keyboard interrupts\n");
  }
  
  // Free game resources
  game_exit(game);
  
  return 0;
}
