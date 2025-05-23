// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include "i8254.h"

// Any header files included below this line should have been created by you

extern uint8_t scancode;
extern vbe_mode_info_t vmi_p;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (on_esc_key)() {
  uint8_t irq_set;
  message msg;
  int ipc_status;

  if (keyboard_subscribe_int(&irq_set) != 0)
    return 1;

  while (scancode != 0x81) { // ESC break code
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error receiving message\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
          }
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0)
    return 1;

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  set_mode(mode);

  sleep(delay);

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if (vg_init(mode) == NULL) {
    return 1;
  }
  
  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
    return 1;
  }

  if (on_esc_key() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  if (vg_init(mode) == NULL) {
    return 1;
  }

  uint16_t width = vmi_p.XResolution / no_rectangles;
  uint16_t height = vmi_p.YResolution / no_rectangles;

  for (uint8_t i = 0; i < no_rectangles; i++) {
    for (uint8_t j = 0; j < no_rectangles; j++) {
      uint32_t color;
      
      if (vmi_p.MemoryModel == 0x06) {
        uint8_t r_first = (first >> vmi_p.RedFieldPosition) & ((1 << vmi_p.RedMaskSize) - 1);
        uint8_t g_first = (first >> vmi_p.GreenFieldPosition) & ((1 << vmi_p.GreenMaskSize) - 1);
        uint8_t b_first = (first >> vmi_p.BlueFieldPosition) & ((1 << vmi_p.BlueMaskSize) - 1);
        
        uint8_t r = (r_first + j * step) % (1 << vmi_p.RedMaskSize);
        uint8_t g = (g_first + i * step) % (1 << vmi_p.GreenMaskSize);
        uint8_t b = (b_first + (i + j) * step) % (1 << vmi_p.BlueMaskSize);
        
        color = (r << vmi_p.RedFieldPosition) | 
                (g << vmi_p.GreenFieldPosition) | 
                (b << vmi_p.BlueFieldPosition);
      }
      else {
        color = (first + (i * no_rectangles + j) * step) % (1 << vmi_p.BitsPerPixel);
      }
      
      if (vg_draw_rectangle(j * width, i * height, width, height, color) != 0) {
        return 1;
      }
    }
  }

  if (on_esc_key() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (vg_init(0x105) == NULL) {
    return 1;
  }

  if (draw_xpm(xpm, x, y) != 0) {
    return 1;
  }

  if (on_esc_key() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int ipc_status;
  message msg;
  uint8_t irq_set_timer, irq_set_kbc;

  uint8_t vertical;
  int8_t direction;
  
  if (xi == xf && yi != yf) {
    vertical = 1;
    direction = (yi < yf) ? 1 : -1;
  } else if (yi == yf && xi != xf) {
    vertical = 0;
    direction = (xi < xf) ? 1 : -1;
  } else {
    return 1;
  }

  if (keyboard_subscribe_int(&irq_set_kbc) != 0) {
    printf("Error subscribing keyboard interrupts\n");
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    printf("Error subscribing timer interrupts\n");
    return 1;
  }

  if (timer_set_frequency(0, fr_rate) != 0) {
    printf("Error setting timer frequency\n");
    return 1;
  }

  if (vg_init(0x105) == NULL) {
    printf("Error initializing video mode\n");
    return 1;
  }
  
  // Draw the initial position
  if (draw_xpm(xpm, xi, yi) != 0) {
    printf("Error drawing xpm\n");
    return 1;
  }

  // Flag to track if we've reached final position
  bool reached_destination = false;

  while (scancode != 0x81) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error receiving message\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbc) {
            kbc_ih();
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            
            // Only update position if not reached destination yet
            if (!reached_destination) {
              if (clear_screen() != 0) {
                printf("Error clearing screen\n");
                return 1;
              }
              
              if (vertical) {
                yi += speed;
                if ((direction > 0 && yi > yf) || (direction < 0 && yi < yf)) {
                  yi = yf;
                  reached_destination = true;
                }
              } else {
                xi += speed;
                if ((direction > 0 && xi > xf) || (direction < 0 && xi < xf)) {
                  xi = xf;
                  reached_destination = true;
                }
              }
              
              if (draw_xpm(xpm, xi, yi) != 0) {
                printf("Error drawing xpm\n");
                return 1;
              }

              if (xi == xf && yi == yf) {
                reached_destination = true;
              }
            }
          }
          break;
      }
    }
  }

  if (vg_exit() != 0) {
    printf("Error exiting video mode\n");
    return 1;
  }

  if (keyboard_unsubscribe_int() != 0) {
    printf("Error unsubscribing keyboard interrupts\n");
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    printf("Error unsubscribing timer interrupts\n");
    return 1;
  }

  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
