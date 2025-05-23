#include "video.h"
#include <lcom/lcf.h>

static void *video_mem[2];
static int videoBufferIndex = 0;

int (set_mode)(uint16_t mode) {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86)); // initialize all fields to zero

  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  reg86.bx = mode | BIT(14);

  if (sys_int86(&reg86) != 0) {
    return 1;
  }

  return 0;
}

int (flip)() {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86)); // initialize all fields to zero

  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x07;
  reg86.cx = 0;
  reg86.dx = videoBufferIndex * vmi_p.YResolution;

  if (sys_int86(&reg86) != 0) {
    return 1;
  }

  videoBufferIndex = (videoBufferIndex + 1) % 2;
  return 0;
}

void* (vg_init)(uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;
  int r;

  if (vbe_get_mode_info(mode, &vmi_p) != 0) {
    return NULL;
  }

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + 2 * vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    return NULL;
  }

  video_mem[0] = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  video_mem[1] = vm_map_phys(SELF, (void *)(mr.mr_base + vram_size), vram_size);

  if (video_mem[0] == MAP_FAILED || video_mem[1] == MAP_FAILED) {
    return NULL;
  }

  if (set_mode(mode) != 0) {
    return NULL;
  }

  return video_mem[videoBufferIndex];
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi_p.XResolution || y >= vmi_p.YResolution) {
    return 1;
  }

  unsigned int index = (y * vmi_p.XResolution + x);
  
  if (vmi_p.BitsPerPixel == 8) {
    uint8_t *video_ptr = (uint8_t *)video_mem[videoBufferIndex];
    video_ptr[index] = (uint8_t)(color & 0xFF);
  }
  else if (vmi_p.BitsPerPixel == 15 || vmi_p.BitsPerPixel == 16) {
    uint16_t *video_ptr = (uint16_t *)video_mem[videoBufferIndex];
    video_ptr[index] = (uint16_t)(color & 0xFFFF);
  }
  else if (vmi_p.BitsPerPixel == 24) {
    uint8_t *video_ptr = (uint8_t *)video_mem[videoBufferIndex];
    index *= 3;
    video_ptr[index] = color & 0xFF;
    video_ptr[index + 1] = (color >> 8) & 0xFF;
    video_ptr[index + 2] = (color >> 16) & 0xFF;
  }
  else if (vmi_p.BitsPerPixel == 32) {
    uint32_t *video_ptr = (uint32_t *)video_mem[videoBufferIndex];
    video_ptr[index] = color;
  }
  
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) {
      return 1;
    }
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      return 1;
    }
  }

  return 0;
}

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_8_8_8, &img);

  for (unsigned i = 0; i < img.height; i++) {
    for (unsigned j = 0; j < img.width; j++) {
      uint32_t color = map[i * img.width + j];
      if (vg_draw_pixel(x + j, y + i, color) != 0) {
        free(map);
        return 1;
      }
    }
  }

  return 0;
}

int (clear_screen)() {
  if (memset(video_mem[videoBufferIndex], 0, vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8)) == NULL) {
    return 1;
  }
  
  return 0;
}
