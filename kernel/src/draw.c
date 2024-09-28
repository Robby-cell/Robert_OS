#include "draw.h"
#include "kernel_glbl.h"

int Krnl_ScreenWidth = 320;
int Krnl_ScreenHeight = 200;

void Krnl_set_video_mode_13h(void) {
  asm volatile (
    "mov $0x13, %%ax\n\t"
    "int $0x10\n"
    :
    :
    : "%ax"
  );
}

