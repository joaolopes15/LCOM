#include "KBC.h"
#include <stdint.h>

int(read_KBC_status)(uint8_t *st){
    return util_sys_inb(0x64, st);
}

int read_KBC_output(uint8_t port, uint8_t *output, uint8_t mouse){
    uint8_t st;
    int tries = 10;

    while(tries > 0){
        if(read_KBC_status(&st) != 0){
            printf("Error reading KBC status\n");
            return 1;
        }
        
        if(st & BIT(0)){
            if(util_sys_inb(port, output) != 0){
                printf("Error reading KBC output\n");
                return 1;
            }

            if((st & BIT(6)) != 0){
                printf("Error timeout\n");
                return 1;
            }
            if((st & BIT(7)) != 0){
                printf("Error parity\n");
                return 1;
            }
            
            if(mouse && !(st & BIT(5))){
                printf("Error mouse output not found\n");
                return 1;
            }
            if(!mouse && (st & BIT(5))){
                printf("Error keyboard output not found\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        tries--;
    }

    return 1;
}

int(write_KBC_command)(uint8_t port, uint8_t command){
    uint8_t st;
    int tries = 40;

    while(tries > 0){
        if(read_KBC_status(&st) != 0){
            printf("Error reading KBC status\n");
            return 1;
        }

        if((st & BIT(1)) == 0){
            if(sys_outb(port, (uint32_t)command) != 0){
                printf("Error writing KBC command\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        tries--;
    }

    return 1;
}
