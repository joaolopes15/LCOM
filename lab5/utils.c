#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;
  *lsb = 0xFF & val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;
  *msb = 0xFF & (val >> 8);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  if (value == NULL)
    return 1;                    // o apontador deve ser válido
  uint32_t val;                  // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val); // val ficará com o valor lido de port
  *value = 0xFF & val;           // value ficará apenas com os primeiros 8 bits do resultado
  return ret;

  return 1;
}
