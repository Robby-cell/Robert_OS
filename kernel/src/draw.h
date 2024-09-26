#ifndef KRNL_KRNL_C_DRAW_H
#define KRNL_KRNL_C_DRAW_H

#include <stdint.h>

// VGA color palette in 13h mode (0-255)
// Standard 16 VGA colors (0-15)
#define BLACK          0x00
#define BLUE           0x01
#define GREEN          0x02
#define CYAN           0x03
#define RED            0x04
#define MAGENTA        0x05
#define BROWN          0x06
#define LIGHT_GRAY     0x07
#define DARK_GRAY      0x08
#define LIGHT_BLUE     0x09
#define LIGHT_GREEN    0x0A
#define LIGHT_CYAN     0x0B
#define LIGHT_RED      0x0C
#define LIGHT_MAGENTA  0x0D
#define YELLOW         0x0E
#define WHITE          0x0F

// Extended colors (16-255) can be customized, typically system-specific
// Here are a few examples from the default palette:
#define BRIGHT_BLUE     0x10
#define BRIGHT_GREEN    0x11
#define BRIGHT_CYAN     0x12
#define BRIGHT_RED      0x13
#define BRIGHT_MAGENTA  0x14
#define BRIGHT_YELLOW   0x15
#define BRIGHT_WHITE    0x16

extern int Krnl_ScreenWidth;
extern int Krnl_ScreenHeight;

extern void Krnl_put_pixel(int x, int y, uint8_t color);

void Krnl_set_video_mode_13h(void);

#endif // KRNL_KRNL_C_DRAW_H

