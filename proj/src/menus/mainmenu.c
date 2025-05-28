#include "mainmenu.h"
#include "../assets/menus/welcome_xpm.h"
#include "../assets/menus/to_xpm.h"
#include "../assets/logo_xpm.h"
#include "../assets/menus/startS_xpm.h"
#include "../assets/menus/startgame_xpm.h"
#include "../assets/menus/howtoplayS_xpm.h"
#include "../assets/menus/howtoplay_xpm.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/exit_xpm.h"
#include "../assets/menus/background_xpm.h"
#include "../drivers/video/video.h"

void mainmenu_process_input(game_t *game, uint8_t scancode) {
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;
    
    if (!is_release) {
        if (scancode == 0x1C) { // enter key
            if (game->main_menu_selected_option == 0) {
                game_change_state(game, STATE_PLAYING);
            } else if (game->main_menu_selected_option == 1) {
                game_change_state(game, STATE_HOW_TO_PLAY);
            } else if (game->main_menu_selected_option == 2) {
                game_change_state(game, STATE_EXIT);
            }
        } else if (scancode == 0x01) { // esc key to exit
            game_change_state(game, STATE_EXIT);
        } else if (key_code == 0x48) { // up arrow
            if (game->main_menu_selected_option > 0) game->main_menu_selected_option--;
        } else if (key_code == 0x50) { // down arrow
            if (game->main_menu_selected_option < 2) game->main_menu_selected_option++;
        }
    }
}

void mainmenu_render(game_t *game) {
    clear_screen();
    draw_xpm((xpm_map_t) background_xpm, 0, 0);
    draw_xpm((xpm_map_t) welcome_xpm, 200, 0);
    draw_xpm((xpm_map_t) to_xpm, 450, 0);
    draw_xpm((xpm_map_t) logo_xpm, 270, 100);
    
    if (game->main_menu_selected_option == 0)
        draw_xpm((xpm_map_t) startS_xpm, 250, 300);
    else
        draw_xpm((xpm_map_t) startgame_xpm, 250, 300);
    
    if (game->main_menu_selected_option == 1)
        draw_xpm((xpm_map_t) howtoplayS_xpm, 250, 400);
    else
        draw_xpm((xpm_map_t) howtoplay_xpm, 250, 400);
    
    if (game->main_menu_selected_option == 2)
        draw_xpm((xpm_map_t) exitS_xpm, 230, 500);
    else
        draw_xpm((xpm_map_t) exit_xpm, 250, 500);
}
