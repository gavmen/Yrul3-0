// src/interrupts.h
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Structure of an IDT entry
struct idt_entry {
    uint16_t base_low;
    uint16_t sel;       // Kernel segment selector
    uint8_t  always0;
    uint8_t  flags;     // Type and attributes
    uint16_t base_high;
} __attribute__((packed));

// Pointer structure for loading the IDT
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Functions to set up the IDT
extern void init_idt(void);
extern void isr_handler(void);

#ifdef __cplusplus
}
#endif

#endif // INTERRUPTS_H