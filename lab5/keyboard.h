#include <lcom/lcf.h>
#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_
#define KEYBOARD_IRQ 1
#define KBD_OUT_BUF 0x60
#define KBD_STAT_REG 0x64

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"



int (keyboard_subscribe_int)(uint8_t *bit_no);

int (keyboard_unsubscribe_int)();

void (kbc_ih)();

#endif
