#include "print.h"
#include "kernel_glbl.h"

void Krnl_print_at(int x, int y, char data) {
  int position = (y * COLUMNS_IN_LINE) + x;
  VidPtr[position] = data;
  VidPtr[position + 1] = 0x07;
}

void kprint(const char* str) {
  unsigned int i = 0;
  while (str[i] != '\0') {
    VidPtr[CurrentLocation++] = str[i++];
    VidPtr[CurrentLocation++] = 0x07;
  }
}

void Krnl_clear_screen(void) {
  unsigned i = 0;
  while (i < SCREENSIZE) {
    VidPtr[i++] = ' ';
    VidPtr[i++] = 0x07;
  }
}
