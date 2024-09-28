#include "mem.h"

void memset(void *ptr, int value, unsigned long count) {
  while (count--) {
    *(char*)ptr = (char)value;
  }
}
