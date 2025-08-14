#include "pic_acknowledge.h"
#include "io.h"
#include "interrupts.h"
#include "types.h"

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;
void remap_pic(void) {
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    
    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();
    
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

void idt_set_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void pic_acknowledge(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

void init_idt(void) {
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, 0, 0, 0);
    }
    
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;
    
    __asm__ volatile("lidt %0" : : "m"(idtp) : "memory");
}
