#include "keyboard.h"

extern void Krnl_keyboard_handler(void);
extern char Krnl_read_port(unsigned short port);
extern void Krnl_write_port(unsigned short port, unsigned char data);
extern void Krnl_load_idt(unsigned long *idt_ptr);

#define IS_STATUS_PRESSED(STATUS) ((STATUS & KS_KEY_DOWN) == KS_KEY_DOWN)
#define IS_STATUS_RELEASED(STATUS) ((STATUS & KS_KEY_RELEASED) == KS_KEY_RELEASED)

void Krnl_keyboard_handler_main(void) {
  unsigned char status;
  char keycode;

  Krnl_write_port(0x20, 0x20);

  status = Krnl_read_port(KEYBOARD_STATUS_PORT);
  keycode = Krnl_read_port(KEYBOARD_DATA_PORT);
  if (keycode < 0) {
    return;
  }

  switch (status) {
    case KS_KEYDOWN: {
      Krnl_keyboard_state[keycode] = (struct ButtonState){
        .key_state = KS_KEYDOWN,
        .keydown = (struct Keydown) {
          .key = keycode,
        },
      };
      break;
    }
    case KS_KEY_RELEASED: {
      Krnl_keyboard_state[keycode] = (struct ButtonState){
        .key_state = KS_KEY_RELEASED,
        .keyup = (struct Keyup) {
          .key = keycode,
        },
      };
      break;
    }

    default: {
      break;
    }
  }

  // if (status & 0x01) {
  //   if (keycode < 0) {
  //     return;
  //   }
  //   Krnl_keyboard_state[keycode] = (struct ButtonState){
  //     .key_state = KS_KEYDOWN,
  //   };
  //   if (keycode == ENTER_KEY_CODE) {
  //     kprint_newline();
  //     return;
  //   }
  //   VidPtr[CurrentLocation++] = KeyboardMap[(unsigned char)keycode];
  //   VidPtr[CurrentLocation++] = 0x07;
  // }
}

int Krnl_is_key_pressed(int keycode) {
  if (keycode < 0 || keycode >= 128) {
    return 0;
  }
  return Krnl_keyboard_state[keycode].key_state == KS_KEYDOWN;
}
