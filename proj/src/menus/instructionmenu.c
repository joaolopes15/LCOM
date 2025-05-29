#include "instructionmenu.h"
#include "../assets/menus/instructions/menuinstructions_xpm.h"
#include "../assets/menus/instructions/pressanykey_xpm.h"
#include "../assets/menus/instructions/instruction_xpm.h"
#include "../assets/menus/instructions/gameplayinstruction_xpm.h"
#include "../assets/menus/instructionBIGGER_xpm.h"
#include "../drivers/video/video.h"

void instructionmenu_process_input(game_t *game, uint8_t scancode) {
    bool is_release = (scancode & 0x80) != 0;
    
    // Return to menu on any key press
    if (!is_release) {
        game_change_state(game, STATE_MENU);
    }
}

void instructionmenu_render(game_t *game) {
    clear_screen();
    Sprite *instruction_sprite = create_sprite((xpm_map_t) instructionBIGGER_xpm);
    Sprite *gameplay_instruction_sprite = create_sprite((xpm_map_t) gameplayinstruction_xpm);
    //Sprite *background_sprite = create_sprite((xpm_map_t) background4_xpm);
    //draw_sprite(background_sprite, 0, 0);
    draw_sprite(instruction_sprite, 250, 0);
    draw_sprite(gameplay_instruction_sprite, 100, 200);
    destroy_sprite(instruction_sprite);
    destroy_sprite(gameplay_instruction_sprite);
    //destroy_sprite(background_sprite);

}
