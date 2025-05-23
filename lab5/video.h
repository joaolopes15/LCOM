#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <lcom/lcf.h>

vbe_mode_info_t vmi_p;

int (set_mode)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

int (clear_screen)();

#endif
