#ifndef KRNL_KRNL_C_PRINT_H
#define KRNL_KRNL_C_PRINT_H

void Krnl_print_at(int x, int y, char data);
void kprint(const char *str);
void Krnl_clear_screen(void);
extern void Krnl_print_decimal(char *buffer, unsigned number);
extern void Krnl_print_hex(char *buffer, unsigned number);

#endif // KRNL_KRNL_C_PRINT_H
