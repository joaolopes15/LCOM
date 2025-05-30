#include "pausemenu.h"
#include "../assets/menus/paused_xpm.h"
#include "../assets/menus/continueS_xpm.h"
#include "../assets/menus/continue_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/retry_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/pausedBIGGER_xpm.h"
#include "../drivers/video/video.h"

void pausemenu_process_input(game_t *game, uint8_t scancode) {
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;
    
    if (!is_release) {
        if (scancode == 0x1C) { // enter key
            if (game->pause_menu_selected_option == 0) {
                game_change_state(game, STATE_PLAYING);
            } else if (game->pause_menu_selected_option == 1) {
                game_change_state(game, STATE_PLAYING);
            } else if (game->pause_menu_selected_option == 2) {
                game_change_state(game, STATE_MENU);
            }
        } else if (key_code == 0x48) { // up arrow
            if (game->pause_menu_selected_option > 0) game->pause_menu_selected_option--;
        } else if (key_code == 0x50) { // down arrow
            if (game->pause_menu_selected_option < 2) game->pause_menu_selected_option++;
        }
    }
}

void pausemenu_render(game_t *game) {
    clear_screen();
    //Sprite *background_sprite = create_sprite((xpm_map_t) background4_xpm);
    Sprite *paused_sprite = create_sprite((xpm_map_t) pausedBIGGER_xpm);
    Sprite *continueS_sprite = create_sprite((xpm_map_t) continueS_xpm);
    Sprite *continue_sprite = create_sprite((xpm_map_t) continue_xpm);
    Sprite *retryS_sprite = create_sprite((xpm_map_t) retryS_xpm);
    Sprite *retry_sprite = create_sprite((xpm_map_t) retry_xpm);
    Sprite *exittomenuS_sprite = create_sprite((xpm_map_t) exittomenuS_xpm);
    Sprite *exittomenu_sprite = create_sprite((xpm_map_t) exittomenu_xpm);
    
    //draw_sprite(background_sprite, 0, 0);
    draw_sprite(paused_sprite, 200, 30);
    
    if (game->pause_menu_selected_option == 0)
        draw_sprite(continueS_sprite, 250, 200);
    else
        draw_sprite(continue_sprite, 250, 200);
    
    if (game->pause_menu_selected_option == 1)
        draw_sprite(retryS_sprite, 250, 300);
    else
        draw_sprite(retry_sprite, 250, 300);
    
    if (game->pause_menu_selected_option == 2)
        draw_sprite(exittomenuS_sprite, 250, 400);
    else
        draw_sprite(exittomenu_sprite, 250, 400);
    
    destroy_sprite(paused_sprite);
    destroy_sprite(continueS_sprite);
    destroy_sprite(continue_sprite);
    destroy_sprite(retryS_sprite);
    destroy_sprite(retry_sprite);
    destroy_sprite(exittomenuS_sprite);
    destroy_sprite(exittomenu_sprite);
    //destroy_sprite(background_sprite);
}
