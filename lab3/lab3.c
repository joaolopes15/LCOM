#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h" 

#define MAKE_CODE 0x80
#define BREAK_CODE 0x81
#define TWO_BYTES 0xE0

extern int counter_KBC;
extern uint8_t scancode;
extern bool expectingSecondByte;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;
  uint8_t irq_set;

  if (keyboard_subscribe_int(&irq_set) != 0) return 1;

  while (scancode != 0x81) {
    
    if ((driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (expectingSecondByte == false){
              kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
            }
            
            //TODO: Exportar a variavel expectingSecondByte para o lab3.c
            //Chamar kbc_print_scancode() é só chamar.
            // MAS ATENCAO: Só chamas isto quando expectingSecondByte for false.
            // POrque há casos que tens de esperar o segundo byte (se receber antes o 0xE0). E nesse caso não chamo logo o kbc_print_scancode()

            // MAS: Para chamar kbd_print_scancode (bool make, uint8_t size, uint8_t *bytes)
            // Tenho de configurar é make code. e Tenho de garantir que passo o size correto.
          }
          
          break;
        default:
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter_KBC)) return 1;

  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
