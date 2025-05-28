#include "instructionmenu.h"
#include "../assets/menus/instructions/menuinstructions_xpm.h"
#include "../assets/menus/instructions/pressanykey_xpm.h"
#include "../assets/menus/instructions/instruction_xpm.h"
#include "../assets/menus/instructions/gameplayinstruction_xpm.h"
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
    draw_xpm((xpm_map_t) instruction_xpm, 250, 0);
    draw_xpm((xpm_map_t) gameplayinstruction_xpm, 100, 200);
    // Uncomment if you want these additional elements:
    // draw_xpm((xpm_map_t) menuinstructions_xpm, 250, 400);
    // draw_xpm((xpm_map_t) pressanykey_xpm, 250, 500);
}
