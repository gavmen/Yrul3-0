#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64
#define KEYBOARD_COMMAND_PORT 0x64

#define KEYBOARD_STATUS_OUT_FULL   0x01
#define KEYBOARD_STATUS_IN_FULL    0x02

#define KEYBOARD_CMD_READ_CONFIG   0x20
#define KEYBOARD_CMD_WRITE_CONFIG  0x60
#define KEYBOARD_CMD_DISABLE_PS2   0xA7
#define KEYBOARD_CMD_ENABLE_PS2    0xA8
#define KEYBOARD_CMD_RESET         0xFF

void keyboard_init(void);
void keyboard_handler(void);
void keyboard_poll(void);
void keyboard_setup_display(void);

extern volatile int keyboard_interrupt_count;

#endif
