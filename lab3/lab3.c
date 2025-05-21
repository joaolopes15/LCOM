#include "i8254.h"
#include "keyboard.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <minix/ipc.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include <stdint.h>

extern uint32_t counter_KBC;
extern uint32_t counter_TIMER;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(kbd_test_scan)() {
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
            if (scancode != 0) {
              kbd_print_scancode(!(scancode & BIT(7)),
                                 scancode == 0xE0 ? 2 : 1, &scancode);
            }
          }
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0)
    return 1;
  if (kbd_print_no_sysinb(counter_KBC) != 0)
    return 1;
  return 0;
}

int(kbd_test_poll)() {
  while (scancode != 0x81) { // ESC break code
    if (read_KBC_output(0x60, &scancode, 0) == 0) {
      if (scancode != 0) {
        kbd_print_scancode(!(scancode & BIT(7)),
                           scancode == 0xE0 ? 2 : 1, &scancode);
      }
    }
    tickdelay(micros_to_ticks(20000)); // Small delay to avoid busy waiting
  }

  if (keyboard_restore() != 0)
    return 1;
  if (kbd_print_no_sysinb(counter_KBC) != 0)
    return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t idle) {
  uint8_t irq_set_TIMER, irq_set_KBC;
  message msg;
  int ipc_status;
  int seconds = 0;

  if (timer_subscribe_int(&irq_set_TIMER) != 0)
    return 1;
  if (keyboard_subscribe_int(&irq_set_KBC) != 0) {
    timer_unsubscribe_int();
    return 1;
  }

  while (scancode != 0x81 && seconds < idle) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error receiving message\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_KBC) {
            kbc_ih();
            if (scancode != 0) {
              kbd_print_scancode(!(scancode & BIT(7)),
                                 scancode == 0xE0 ? 2 : 1, &scancode);
              seconds = 0; // Reset idle counter on keypress
              counter_TIMER = 0;
            }
          }
          if (msg.m_notify.interrupts & irq_set_TIMER) {
            timer_int_handler();
            if (counter_TIMER % 60 == 0) { // 60 interrupts = 1 second
              seconds++;
            }
          }
          break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0)
    return 1;
  if (keyboard_unsubscribe_int() != 0)
    return 1;
  if (kbd_print_no_sysinb(counter_KBC) != 0)
    return 1;
  return 0;
}
