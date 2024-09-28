#include "game.h"
#include "draw.h"
#include "random.h"
#include "print.h"
#include "kernel_glbl.h"
#include "keyboard.h"

extern void Krnl_keyboard_handler(void);
extern char Krnl_read_port(unsigned short port);
extern void Krnl_write_port(unsigned short port, unsigned char data);
extern void Krnl_load_idt(unsigned long *idt_ptr);

/**
 * Extern vars, must be init
 */
unsigned CurrentLocation = 0;
char* VidPtr = (char*)0xb8000;
struct ButtonState Krnl_keyboard_state[BUTTONSTATE_SIZE];

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

  for (int i = 0; i < (sizeof(Krnl_keyboard_state) / sizeof(Krnl_keyboard_state[0])); ++i) {
    Krnl_keyboard_state[i] = (struct ButtonState) {
      .key_state = KS_KEY_RELEASED,
      .keyup = (struct Keyup) {
        .key = i,
      },
    };
  }
}

static void Krnl_kb_init(void) {
  Krnl_write_port(0x21, 0xFD);
}

void kprint_newline(void) {
  const unsigned line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
  CurrentLocation = CurrentLocation + (line_size - CurrentLocation % (line_size));
}

void kmain(void) {
	// const char *str = "my first kernel with keyboard support";
  // Krnl_clear_screen();

  // char buffer[20];
  // Krnl_print_hex(buffer, 42343);

	// kprint(str);
  // kprint(buffer);
	// kprint_newline();
	// kprint_newline();

  // Krnl_print_at(30, 30, 'h');

	Krnl_idt_init();
	Krnl_kb_init();

  Krnl_begin_game();

	while(1);
}

