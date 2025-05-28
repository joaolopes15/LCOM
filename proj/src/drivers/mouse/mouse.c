#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
#include "../keyboard/KBC.h"

struct packet m_packet;
int m_hook = 2;

int byte_idx = 0;
uint8_t m_bytes[3];
uint8_t curr_byte;

int mouse_subscribe_int(uint8_t *bit_no){
  if(bit_no == NULL) return 1;
    *bit_no = BIT(m_hook);
    return sys_irqsetpolicy(12 , IRQ_REENABLE | IRQ_EXCLUSIVE, &m_hook);
}

int mouse_unsubscribe_int(){
  return sys_irqrmpolicy(&m_hook);
}

void (mouse_ih)() {
  if(read_KBC_output(0x60, &curr_byte, 1) != 0) {
        // If read fails, don't update curr_byte
        return;
    }
}

void packet_assembly() {
  if (byte_idx == 0 && (curr_byte & FIRST_BYTE)) { 
    m_bytes[byte_idx] = curr_byte;
    byte_idx++;
  }
  else if (byte_idx > 0 && byte_idx < 3) { 
    m_bytes[byte_idx] = curr_byte;
    byte_idx++;
  }
  else if (byte_idx == 0 && !(curr_byte & FIRST_BYTE)) {
    byte_idx = 0;
  }
}

void m_assemble_packet() {
  
  m_packet.bytes[0] = m_bytes[0];
  m_packet.bytes[1] = m_bytes[1];
  m_packet.bytes[2] = m_bytes[2];

  m_packet.lb = (m_bytes[0] & MOUSE_LB) != 0;
  m_packet.mb = (m_bytes[0] & MOUSE_MB) != 0;
  m_packet.rb = (m_bytes[0] & MOUSE_RB) != 0;
  
  m_packet.x_ov = (m_bytes[0] & MOUSE_X_OVERFLOW) != 0;
  m_packet.y_ov = (m_bytes[0] & MOUSE_Y_OVERFLOW) != 0;
  
  m_packet.delta_x = (int16_t)(m_bytes[1]);
  if (m_bytes[0] & MOUSE_X_SIGNAL) {
    m_packet.delta_x -= 256; 
  }
  
  m_packet.delta_y = (int16_t)(m_bytes[2]);
  if (m_bytes[0] & MOUSE_Y_SIGNAL) {
    m_packet.delta_y -= 256;
  }
  
  byte_idx = 0;
}

int (m_write)(uint8_t command) {

  uint8_t response;
  int rem_attemps = 10;

  while (rem_attemps > 0) {
    
    if (write_KBC_command(0x64, WRITE_BYTE_MOUSE) != 0) {
      return 1;  
    }
    
    if (write_KBC_command(0x60, command) != 0) {
      return 1;
    }
    
    tickdelay(micros_to_ticks(20000));
    
    if (util_sys_inb(0x60, &response) != 0) {
      return 1; 
    }
    
    if (response == ACK) {
      return 0; 
    }
    
    if (response != NACK && rem_attemps == 0) {
      break;  
    }
    
    rem_attemps--;
  }
  
  return 1;  // Return error if all attempts failed
}