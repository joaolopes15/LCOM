#include "background.h"
#include "../assets/menus/background4_xpm.h"

// External declarations for video memory
extern void* video_mem[];
extern int currentDrawBuffer;
extern vbe_mode_info_t vmi_p;

int draw_background() {
    xpm_image_t img;
    
    uint8_t *map = xpm_load(background4_xpm, XPM_8_8_8_8, &img);

    if (map == NULL) {
        return 1;
    }

    if (img.width != vmi_p.XResolution || img.height != vmi_p.YResolution) {
        free(map);
        return 1;
    }

    uint8_t *video_ptr = (uint8_t *)video_mem[currentDrawBuffer];
    unsigned bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
    
    for (unsigned i = 0; i < img.height; i++) {
        for (unsigned j = 0; j < img.width; j++) {
            unsigned src_index = (i * img.width + j) * 4;
            unsigned dst_index = (i * vmi_p.XResolution + j) * bytes_per_pixel;
            
            uint8_t red = map[src_index];
            uint8_t green = map[src_index + 1];
            uint8_t blue = map[src_index + 2];
            uint8_t alpha = map[src_index + 3];
            
            if (vmi_p.BitsPerPixel == 8) {
                video_ptr[dst_index] = (red >> 5) << 5 | (green >> 5) << 2 | (blue >> 6);
            }
            else if (vmi_p.BitsPerPixel == 16) {
                uint16_t color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
                *((uint16_t*)(video_ptr + dst_index)) = color;
            }
            else if (vmi_p.BitsPerPixel == 24 || vmi_p.BitsPerPixel == 32) {
                video_ptr[dst_index] = blue;
                video_ptr[dst_index + 1] = green;
                video_ptr[dst_index + 2] = red;
                if (vmi_p.BitsPerPixel == 32) {
                    video_ptr[dst_index + 3] = alpha;
                }
            }
        }
    }

    free(map);
    return 0;
}
