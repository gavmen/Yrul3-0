// src/interrupts.h

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#define IDT_SIZE 256
#define KEYBOARD_IRQ 0x21

// IDT Entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

// IDT Pointer structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function prototypes
void init_idt();
void remap_pic();
void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags);
void keyboard_handler();

#endif // INTERRUPTS_H
