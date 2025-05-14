#ifndef __VBE_H
#define __VBE_H

#define VBE_768p_INDEXED 0x105 // 1024x768, Indexed, Bits per pixel = 8
#define VBE_480p_DC 0x110 // 640x480, Direct Color, Bits per pixel = 15 ((1:)5:5:5)
#define VBE_600p_DC 0x115 // 800x600, Direct Color, Bits per pixel = 24 (8:8:8)
#define VBE_1024p_DC 0x11A // 1024x768, Direct Color, Bits per pixel = 16 (5:6:5)
#define VBE_864p_DC 0x14C // 1024x768, Direct Color, Bits per pixel = 32 ((8:)8:8:8)

#define VBE_MODE_SET 0x4F02
#define VBE_LINEAR_FB BIT(14)

#define DIRECT_COLOR 0x06
#define INDEXED_COLOR 0x105

#define BIOS_SET_VIDEOMODE 0x00
#define BIOS_VIDEOCARD_SERV 0x10

#endif
