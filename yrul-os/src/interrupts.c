// src/interrupts.c

#include "pic_acknowledge.h"
#include "io.h"
//#include "scancode.h"
#include "interrupts.h"
#include <stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY 0xB8000
// #define KEYBOARD_IRQ 1

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

extern void load_idt(struct idt_ptr *idt_ptr);
extern void pic_acknowledge(uint8_t irq);

// Scancode to ASCII map for alphanumeric keys
const char scancode[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 0 - 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', /* 16 - 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 30 - 39 */
    '\'', '`', 0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 44 - 49 */
    'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0,  /* Alt */
    ' ', /* Space bar */
    0,  /* Caps lock */
    /* ... More keys to be added ... */
};

// Remap the PIC to avoid conflicts with CPU exceptions
void remap_pic() {
    outb(0x20, 0x11); // Start initialization for master PIC
    outb(0xA0, 0x11); // Start initialization for slave PIC
    outb(0x21, 0x20); // Master PIC vector offset (32)
    outb(0xA1, 0x28); // Slave PIC vector offset (40)
    outb(0x21, 0x04); // Tell the master PIC that there is a slave PIC at IRQ2
    outb(0xA1, 0x02); // Tell the slave PIC its cascade identity
    outb(0x21, 0x01); // Set both PICs to 8086 mode
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  // Enable all interrupts on master PIC
    outb(0xA1, 0x0);  // Enable all interrupts on slave PIC
}

// Set up an IDT entry
void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

// Keyboard interrupt handler function
void keyboard_handler() {
    static int position = 0;
    unsigned short *video_memory = (unsigned short *)VIDEO_MEMORY;

    // Debug marker to confirm handler is called
    video_memory[0] = (0x0F << 8) | 'K';  // Display 'K' in the top-left corner

    uint8_t scancode = inb(0x60);  // Read scancode from keyboard port
    if (scancode < 128) {          // Ignore key releases (scancode >= 128)
        video_memory[position++] = (0x0F << 8) | (scancode + '0');  // Display raw scancode as character
        if (position >= SCREEN_WIDTH * SCREEN_HEIGHT) {
            position = 0;  // Wrap around if end of screen reached
        }
    }

    // Acknowledge the interrupt once at the end
    // pic_acknowledge(KEYBOARD_IRQ);
}

// Initialize the IDT with the keyboard handler
void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (uint32_t)&idt;

    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    // Set keyboard interrupt handler
    idt_set_entry(KEYBOARD_IRQ, (uint32_t)keyboard_handler, 0x08, 0x8E);

    // Load the IDT
    load_idt(&idtp);
}
