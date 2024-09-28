#ifndef KRNL_KRNL_C_KEYBOARD_H
#define KRNL_KRNL_C_KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

void Krnl_keyboard_handler_main(void);

enum KeyState {
  KS_KEYDOWN = 0x01,
  KS_KEY_RELEASED = 0x80,
};

struct Keydown {
  int key;
};
struct Keyup {
  int key;
};

struct ButtonState {
  enum KeyState key_state;
  union {
    struct Keydown keydown;
    struct Keyup keyup;
  };
};

#define IS_KEY_PRESSED(STATE, KEYCODE)                                         \
  (((STATE)->key_state & KS_KEYDOWN) && ((STATE)->keydown.key == KEYCODE))
#define IS_KEY_RELEASED(STATE, KEYCODE)                                        \
  ((((STATE)->key_state | KS_KEY_RELEASED) == KEYCODE | KS_KEY_RELEASED) &&    \
   ((STATE)->keyup.key == KEYCODE))

#define BUTTONSTATE_SIZE 128
extern struct ButtonState Krnl_keyboard_state[BUTTONSTATE_SIZE];

int Krnl_is_key_pressed(int keycode);

#endif // KRNL_KRNL_C_KEYBOARD_H
