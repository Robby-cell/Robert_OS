#ifndef KRNL_KRNL_C_KERNEL_GLBL_H
#define KRNL_KRNL_C_KERNEL_GLBL_H

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES)

#define ENTER_KEY_CODE 0x1C

extern int Krnl_ScreenWidth;
extern int Krnl_ScreenHeight;

extern unsigned CurrentLocation;
extern char *VidPtr;

extern int Krnl_keyboard_queue[10];

#endif // KRNL_KRNL_C_KERNEL_GLBL_H
