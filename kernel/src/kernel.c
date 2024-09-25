#include "keyboard_map.inl.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES)

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C

extern unsigned char KeyboardMap[128];
extern void Krnl_keyboard_handler(void);
extern char Krnl_read_port(unsigned short port);
extern void Krnl_write_port(unsigned short port, unsigned char data);
extern void Krnl_load_idt(unsigned long *idt_ptr);

static unsigned CurrentLocation = 0;
static char* VidPtr = (char*)0xb8000;

struct Krnl_IdtEntry {
  unsigned short offset_lowerbits;
  unsigned short selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short offset_higherbits;
};

static struct Krnl_IdtEntry IDT[IDT_SIZE];

static void Krnl_idt_init(void) {
  unsigned long keyboard_addr;
  unsigned long idt_addr;
  unsigned long idt_ptr[2];

  keyboard_addr = (unsigned long)Krnl_keyboard_handler;
  IDT[0x21].offset_lowerbits = keyboard_addr & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_addr & 0xffff0000) >> 16;

  // BEGIN INIT
  Krnl_write_port(0x20 , 0x11);
	Krnl_write_port(0xA0 , 0x11);

  // SETUP CASCADING
  Krnl_write_port(0x21 , 0x20);
	Krnl_write_port(0xA1 , 0x28);

  // ENVIRONMENT INFO
  Krnl_write_port(0x21 , 0x01);
	Krnl_write_port(0xA1 , 0x01);
  // INIT FINISHED

  // MASK INTERRUPTS
  Krnl_write_port(0x21 , 0xff);
	Krnl_write_port(0xA1 , 0xff);

  idt_addr = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof(struct Krnl_IdtEntry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	idt_ptr[1] = idt_addr >> 16 ;

	Krnl_load_idt(idt_ptr);
}

static void Krnl_kb_init(void) {
  Krnl_write_port(0x21, 0xFD);
}

void kprint(const char* str) {
  unsigned int i = 0;
  while (str[i] != '\0') {
    VidPtr[CurrentLocation++] = str[i++];
    VidPtr[CurrentLocation++] = 0x07;
  }
}

void kprint_newline(void) {
  const unsigned line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
  CurrentLocation = CurrentLocation + (line_size - CurrentLocation % (line_size));
}

void Krnl_clear_screen(void) {
  unsigned i = 0;
  while (i < SCREENSIZE) {
    VidPtr[i++] = ' ';
    VidPtr[i++] = 0x07;
  }
}

void Krnl_keyboard_handler_main(void) {
  unsigned char status;
  char keycode;

  Krnl_write_port(0x20, 0x20);

  status = Krnl_read_port(KEYBOARD_STATUS_PORT);

  if (status & 0x01) {
    keycode = Krnl_read_port(KEYBOARD_DATA_PORT);
    if (keycode < 0) {
      return;
    }

    if (keycode == ENTER_KEY_CODE) {
      kprint_newline();
      return;
    }

    VidPtr[CurrentLocation++] = KeyboardMap[(unsigned char)keycode];
    VidPtr[CurrentLocation++] = 0x07;
  }
}

void kmain(void)
{
	const char *str = "my first kernel with keyboard support";

	Krnl_clear_screen();
	kprint(str);
	kprint_newline();
	kprint_newline();

	Krnl_idt_init();
	Krnl_kb_init();

	while(1);
}

