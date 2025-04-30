#include "keyboard.h"
#include "i8042.h"
#include "KBC.h"

int hook_id = 0;
int TIMER0_IRQ = 1; 
uint8_t status;
uint8_t data;
uint8_t bytes[2] = {0, 0};
bool expectingSecondByte = false;
int contadorBytes = 0;
uint8_t scancode;
int counter_KBC = 0;

int(keyboard_subscribe_int)(uint8_t *bit_no) {      
  if (bit_no == NULL)
    return 1;             // o apontador deve ser válido
  *bit_no = BIT(hook_id); // a função que chamou esta deve saber qual é a máscara a utilizar
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1; // subscrição das interrupções
  return 0;
}

int(keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0)
    return 1; // desligar as interrupções
  return 0;
}

void (kbc_ih)() {

  if (read_KBC_output(KBC_OUT_CMD, &scancode) != 0) printf("Error: Could not read scancode!\n");
  // TODO: Read OutBUF (0x60)
  // TODO: Depois de ter o OUTBUF lido:
  // 1. Verificar se é um 0xE0
  // 2. Se for, esperar pelo segundo byte
  // 3. Se não for, guardar o byte
  // Dependendo do valor de expectingSecondByte, guardar o byte no lugar certo.
  // O lugar certo é:
  // 1. Se for o primeiro byte, guardar em bytes[0]
  // 2. Se for o segundo byte, guardar em bytes[1]
  // Não esquecer que o lugar certo é "apontado" por contadorBytes Ou é 0 ou é 1.
  // Se for 0, guardar em bytes[0]. Se for 1, guardar em bytes[1].
  // Nao esquecer que temos de resetar o contador de bytes para 0.


  // SO DEPOIS DISTO PASSAR TESTE 1 / TESTE 2 / TESTE 3 é que pensamos na tralha do "verificar o status register" se há erros de paridade, se out buf está cheio. Proteger a leitura com um ciclo de 5 tentativas. Se falhar 1 tentativa, chamar tickdelays(20) e tentar outra vez
}

