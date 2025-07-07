// src/interrupts_clean.c
// Clean interrupt system for Yrul OS

#include "pic_acknowledge.h"
#include "io.h"
#include "interrupts.h"
#include "types.h"

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// PIC remapping and configuration
void remap_pic(void) {
    // Initialize PICs
    outb(0x20, 0x11);  // Start init sequence
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    
    // Set vector offsets
    outb(0x21, 0x20);  // Master PIC at 0x20
    io_wait();
    outb(0xA1, 0x28);  // Slave PIC at 0x28
    io_wait();
    
    // Configure cascade
    outb(0x21, 0x04);  // Master has slave at IRQ2
    io_wait();
    outb(0xA1, 0x02);  // Slave cascade identity
    io_wait();
    
    // Set 8086 mode
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();
    
    // Enable keyboard IRQ (IRQ1 only)
    outb(0x21, 0xFD);  // Enable IRQ1: 11111101
    outb(0xA1, 0xFF);  // Disable all slave IRQs
}

// Set IDT entry
void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

// Initialize IDT
void init_idt(void) {
    // Clear IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, 0, 0, 0);
    }
    
    // Set up IDT pointer
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;
    
    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idtp) : "memory");
}
