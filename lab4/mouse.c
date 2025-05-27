#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
#include "KBC.h"

uint8_t scancode = 1;
struct packet m_packet;
int m_hook = 2;


mouse_subscribe_int(uint8_t *bit_no){
  if(bit_no == NULL) return 1;
    *bit_no = BIT(m_hook);
    return sys_irqsetpolicy(12 , IRQ_REENABLE | IRQ_EXCLUSIVE, &m_hook);
}

mouse_unsubscribe_int(){
  return sys_irqrmpolicy(&m_hook);
}

void mouse_ih(void) {
  if(read_KBC_output(0x60, &scancode, 1) != 0) {
        scancode = 1;
        return;
    }
}