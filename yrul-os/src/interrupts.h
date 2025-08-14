#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "io.h"

#define IDT_SIZE 256
#define KEYBOARD_IRQ 1

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();
void remap_pic();
void idt_set_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void pic_acknowledge(uint8_t irq);

extern void keyboard_interrupt_wrapper();

#endif
