#include "io.h"
#include "interrupts.h"
#include "keyboard.h"
#include "memory.h"
#include "timer.h"
#include "task.h"

extern void init_idt(void);
extern void remap_pic(void);
extern void keyboard_interrupt_wrapper(void);
extern void timer_interrupt_wrapper(void);
void print_message(const char *message, int row) {
    if (row < 0 || row >= 25 || !message) return;
    
    unsigned short *video_memory = (unsigned short *)0xB8000;
    unsigned char color = 0x0F;
    int position = row * 80;
    
    for (int i = 0; message[i] != '\0' && i < 80; ++i) {
        video_memory[position + i] = (color << 8) | message[i];
    }
}

void clear_screen(void) {
    unsigned short *video_memory = (unsigned short *)0xB8000;
    unsigned short blank = (0x0F << 8) | ' ';
    
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = blank;
    }
}

void kernel_main(void) {
    clear_screen();
    
    print_message("Yrul OS - Simple Operating System", 0);
    print_message("=================================", 1);
    print_message("", 2);
    print_message("Initializing system components...", 3);
    
    print_message("Setting up PIC...", 4);
    remap_pic();
    print_message("PIC configured", 5);
    
    print_message("Setting up IDT...", 6);
    init_idt();
    print_message("IDT loaded", 7);
    
    print_message("Initializing memory...", 8);
    mem_init();
    print_message("Memory manager ready", 9);
    
    print_message("Initializing timer...", 10);
    timer_init();
    print_message("Timer configured", 11);
    
    print_message("Initializing scheduler...", 12);
    task_init();
    print_message("Scheduler ready", 13);
    
    print_message("Initializing keyboard...", 14);
    keyboard_init();
    print_message("Keyboard ready", 15);
    
    print_message("Setting up interrupts...", 16);
    idt_set_entry(0x20, (uint32_t)timer_interrupt_wrapper, 0x08, 0x8E);
    idt_set_entry(0x21, (uint32_t)keyboard_interrupt_wrapper, 0x08, 0x8E);
    print_message("Interrupt handlers registered", 17);
    
    print_message("Enabling interrupts...", 18);
    __asm__ volatile ("sti");
    print_message("System ready!", 19);
    
    print_message("", 20);
    print_message("=========================================", 21);
    print_message("YRUL OS COMMAND LINE", 22);
    print_message("=========================================", 23);
    
    keyboard_setup_display();
    
    while (1) {
        __asm__ volatile("hlt");
    }
}
