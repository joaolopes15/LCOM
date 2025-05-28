#include "gameovermenu.h"
#include "../assets/menus/gameover_xpm.h"
#include "../assets/menus/highscore_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/retry_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/exit_xpm.h"
#include "../drivers/video/video.h"

void gameovermenu_process_input(game_t *game, uint8_t scancode) {
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;
    
    if (!is_release) {
        if (scancode == 0x1C) { // enter key
            if (game->game_over_selected_option == 0) {
                game->barra->x = 350;
                game->barra->y = 500;
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
    clear_screen();
    draw_xpm((xpm_map_t) gameover_xpm, 250, 100);
    draw_xpm((xpm_map_t) highscore_xpm, 250, 200);
    
    if (game->game_over_selected_option == 0)
        draw_xpm((xpm_map_t) retryS_xpm, 250, 300);
    else
        draw_xpm((xpm_map_t) retry_xpm, 250, 300);
    
    if (game->game_over_selected_option == 1)
        draw_xpm((xpm_map_t) exittomenuS_xpm, 250, 400);
    else
        draw_xpm((xpm_map_t) exittomenu_xpm, 250, 400);
    
    if (game->game_over_selected_option == 2)
        draw_xpm((xpm_map_t) exitS_xpm, 250, 500);
    else
        draw_xpm((xpm_map_t) exit_xpm, 250, 500);
}
