#include "backgroundhandler.h"
#include "../assets/menus/background2_xpm.h"

void draw_sprite_to_memory(Sprite *sprite, int x, int y, uint8_t *video_mem, uint16_t screen_width, uint16_t screen_height) {
    if (!sprite || !video_mem) return;
    
    for (int i = 0; i < sprite->height; ++i) {
        int draw_y = y + i;
        if (draw_y < 0 || draw_y >= screen_height) continue;
        
        for (int j = 0; j < sprite->width; ++j) {
            int draw_x = x + j;
            if (draw_x < 0 || draw_x >= screen_width) continue;
            
            uint32_t color = sprite->map[i * sprite->width + j];
            // Only draw if not transparent (alpha != 0)
            if (color >> 24) {
                uint32_t *pixel = (uint32_t *)(video_mem + (draw_y * screen_width + draw_x) * 4);
                *pixel = color;
            }
        }
    }
}

void draw_background_to_memory(uint8_t *video_mem, uint16_t screen_width, uint16_t screen_height) {
    if (!video_mem) return;
    
    Sprite *background = create_sprite((xpm_map_t)background2_xpm);
    if (background) {
        draw_sprite_to_memory(background, 0, 0, video_mem, screen_width, screen_height);
        destroy_sprite(background);
    }
}