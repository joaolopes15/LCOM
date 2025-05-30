#include "gameovermenu.h"
#include "../assets/menus/gameover_xpm.h"
#include "../assets/menus/highscore_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/retry_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/exit_xpm.h"
#include "../assets/menus/gameoverBIGGER_xpm.h"
#include "../drivers/video/video.h"


void gameovermenu_process_input(game_t *game, uint8_t scancode) {
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;
    
    if (!is_release) {
        if (scancode == 0x1C) { // enter key
            if (game->game_over_selected_option == 0) {
                game_change_state(game, STATE_PLAYING);
            } else if (game->game_over_selected_option == 1) {
                game_change_state(game, STATE_MENU);
            } else if (game->game_over_selected_option == 2) {
                game_change_state(game, STATE_EXIT);
            }
        } else if (key_code == 0x48) { // up arrow
            if (game->game_over_selected_option > 0) game->game_over_selected_option--;
        } else if (key_code == 0x50) { // down arrow
            if (game->game_over_selected_option < 2) game->game_over_selected_option++;
        }
    }
}

void gameovermenu_render(game_t *game) {
    //Sprite *background_sprite = create_sprite((xpm_map_t) background4_xpm);
    Sprite *gameover_sprite = create_sprite((xpm_map_t) gameoverBIGGER_xpm);
    Sprite *highscore_sprite = create_sprite((xpm_map_t) highscore_xpm);
    Sprite *retry_sprite = create_sprite((xpm_map_t) retry_xpm);
    Sprite *retryS_sprite = create_sprite((xpm_map_t) retryS_xpm);
    Sprite *exittomenu_sprite = create_sprite((xpm_map_t) exittomenu_xpm);
    Sprite *exittomenuS_sprite = create_sprite((xpm_map_t) exittomenuS_xpm);
    Sprite *exit_sprite = create_sprite((xpm_map_t) exit_xpm);
    Sprite *exitS_sprite = create_sprite((xpm_map_t) exitS_xpm);
    clear_screen();
    //draw_sprite(background_sprite, 0, 0);
    draw_sprite(gameover_sprite, 250, 50);
    draw_sprite(highscore_sprite, 250, 200);
    
    if (game->game_over_selected_option == 0)
        draw_sprite(retryS_sprite, 250, 300);
    else
        draw_sprite(retry_sprite, 250, 300);
    
    if (game->game_over_selected_option == 1)
        draw_sprite(exittomenuS_sprite, 250, 400);
    else
        draw_sprite(exittomenu_sprite, 250, 400);
    
    if (game->game_over_selected_option == 2)
        draw_sprite(exitS_sprite, 220, 500);
    else
        draw_sprite(exit_sprite, 250, 500);
    
    //destroy_sprite(gameover_sprite);
    destroy_sprite(highscore_sprite);
    destroy_sprite(retry_sprite);
    destroy_sprite(retryS_sprite);
    destroy_sprite(exittomenu_sprite);
    destroy_sprite(exittomenuS_sprite);
    destroy_sprite(exit_sprite);
    destroy_sprite(exitS_sprite);
}
