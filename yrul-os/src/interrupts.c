// src/interrupts.c

#include <stdint.h>
#include "io.h"
#include "interrupts.h"

#define IDT_SIZE 256
#define KEYBOARD_IRQ 0x21
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY 0xB8000

void remap_pic() {
    // Send initialization command to the PICs
    outb(0x20, 0x11); // Start initialization for master PIC
    outb(0xA0, 0x11); // Start initialization for slave PIC

    // Set the offset for the interrupts in the IDT
    outb(0x21, 0x20); // Master PIC vector offset (32)
    outb(0xA1, 0x28); // Slave PIC vector offset (40)

    // Tell the master PIC that there is a slave PIC at IRQ2 (0x04)
    outb(0x21, 0x04);
    // Tell the slave PIC its cascade identity (0x02)
    outb(0xA1, 0x02);

    // Set both PICs to operate in 8086/88 (MCS-80/85) mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Enable all interrupts on the PICs (clear the interrupt mask)
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

// IDT entry structure
// struct idt_entry {
//     uint16_t base_low;
//     uint16_t sel;        // Kernel segment selector
//     uint8_t  always0;
//     uint8_t  flags;
//     uint16_t base_high;
// } __attribute__((packed));

// IDT pointer structure
// struct idt_ptr {
//     uint16_t limit;
//     uint32_t base;
// } __attribute__((packed));

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// Function to load IDT (implemented in assembly)
extern void load_idt(struct idt_ptr *idt_ptr);

// Set an entry in the IDT
void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

// External function to acknowledge the interrupt in PIC (implemented in assembly)
extern void pic_acknowledge(uint8_t irq);

// Keyboard interrupt handler function
void keyboard_handler() {
    static int position = 0;
    unsigned short *video_memory = (unsigned short *)0xB8000;
    video_memory[position++] = (0x0F << 8) | '*'; // Write '*' at the current position

    if (position >= 80 * 25) {
        position = 0; // Wrap around when reaching the end of the screen
    }

    // Acknowledge the interrupt
    pic_acknowledge(KEYBOARD_IRQ);
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
