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
#include "../assets/menus/welcomeBIGGER_xpm.h"
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
    draw_background();
    //Sprite *background_sprite = create_sprite((xpm_map_t) background4_xpm);
    Sprite *welcome_sprite = create_sprite((xpm_map_t) welcomeBIGGER_xpm);
    Sprite *to_sprite = create_sprite((xpm_map_t) to_xpm);
    Sprite *logo_sprite = create_sprite((xpm_map_t) logo_xpm);
    Sprite *startS_sprite = create_sprite((xpm_map_t) startS_xpm);
    Sprite *startgame_sprite = create_sprite((xpm_map_t) startgame_xpm);
    Sprite *howtoplayS_sprite = create_sprite((xpm_map_t) howtoplayS_xpm);
    Sprite *howtoplay_sprite = create_sprite((xpm_map_t) howtoplay_xpm);
    Sprite *exitS_sprite = create_sprite((xpm_map_t) exitS_xpm);
    Sprite *exit_sprite = create_sprite((xpm_map_t) exit_xpm);
    //draw_sprite(background_sprite, 0, 0);
    draw_sprite(welcome_sprite, 200, 0);
    draw_sprite(to_sprite, 470, 20);
    draw_sprite(logo_sprite, 270, 100);
    if (game->main_menu_selected_option == 0)
        draw_sprite(startS_sprite, 250, 300);
    else
        draw_sprite(startgame_sprite, 250, 300);
    if (game->main_menu_selected_option == 1)
        draw_sprite(howtoplayS_sprite, 250, 400);
    else
        draw_sprite(howtoplay_sprite, 250, 400);
    if (game->main_menu_selected_option == 2)
        draw_sprite(exitS_sprite, 230, 500);
    else
        draw_sprite(exit_sprite, 250, 500);
    //destroy_sprite(background_sprite);
    destroy_sprite(welcome_sprite);
    destroy_sprite(to_sprite);
    destroy_sprite(logo_sprite);
    destroy_sprite(startS_sprite);
    destroy_sprite(startgame_sprite);
    destroy_sprite(howtoplayS_sprite);
    destroy_sprite(howtoplay_sprite);
    destroy_sprite(exitS_sprite);
    destroy_sprite(exit_sprite);
}
