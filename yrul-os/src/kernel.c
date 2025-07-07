// src/kernel_clean.c
// Simplified and clean Yrul OS kernel

#include "io.h"
#include "interrupts.h"
#include "keyboard.h"

// Function declarations
extern void init_idt(void);
extern void remap_pic(void);
extern void keyboard_interrupt_wrapper(void);

// Simple screen output
void print_message(const char *message, int row) {
    unsigned short *video_memory = (unsigned short *)0xB8000;
    unsigned char color = 0x0F; // White on black
    int position = row * 80;
    
    for (int i = 0; message[i] != '\0' && i < 80; ++i) {
        video_memory[position + i] = (color << 8) | message[i];
    }
}

void clear_screen(void) {
    unsigned short *video_memory = (unsigned short *)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (0x0F << 8) | ' ';
    }
}

// Main kernel function
void kernel_main(void) {
    // Initialize display
    clear_screen();
    
    // Boot messages
    print_message("Yrul OS - Simple Operating System", 0);
    print_message("=================================", 1);
    print_message("", 2);
    print_message("Initializing system components...", 3);
    
    // Initialize PIC
    print_message("Setting up PIC...", 4);
    remap_pic();
    print_message("PIC configured", 5);
    
    // Initialize IDT
    print_message("Setting up IDT...", 6);
    init_idt();
    print_message("IDT loaded", 7);
    
    // Initialize keyboard
    print_message("Initializing keyboard...", 8);
    keyboard_init();
    print_message("Keyboard ready", 9);
    
    // Set up keyboard interrupt
    print_message("Setting up keyboard interrupt...", 10);
    idt_set_entry(0x21, (uint32_t)keyboard_interrupt_wrapper, 0x08, 0x8E);
    print_message("Interrupt handler registered", 11);
    
    // Enable interrupts
    print_message("Enabling interrupts...", 12);
    __asm__ volatile ("sti");
    print_message("System ready!", 13);
    
    // User interface
    print_message("", 14);
    print_message("=========================================", 15);
    print_message("YRUL OS COMMAND LINE", 16);
    print_message("Type commands below:", 17);
    print_message("- Characters appear as you type", 18);
    print_message("- Press Enter to execute command", 19);
    print_message("- Use Backspace to correct mistakes", 20);
    print_message("=========================================", 21);
    
    // Set up input area
    keyboard_setup_display();
    
    // Main system loop
    int heartbeat = 0;
    int last_interrupt_count = 0;
    int polling_mode = 0;
    
    while (1) {
        heartbeat++;
        
        // Heartbeat indicator
        if (heartbeat % 200000 == 0) {
            unsigned short *video = (unsigned short *)0xB8000;
            video[24 * 80 + 75] = (0x07 << 8) | ('0' + ((heartbeat / 200000) % 10));
        }
        
        // Check if interrupts are working
        if (heartbeat % 1000000 == 0) {
            if (keyboard_interrupt_count == last_interrupt_count) {
                // Switch to polling mode
                if (!polling_mode) {
                    polling_mode = 1;
                    unsigned short *video = (unsigned short *)0xB8000;
                    video[24 * 80 + 70] = (0x0C << 8) | 'P';
                    video[24 * 80 + 71] = (0x0C << 8) | 'O';
                    video[24 * 80 + 72] = (0x0C << 8) | 'L';
                    video[24 * 80 + 73] = (0x0C << 8) | 'L';
                }
            } else {
                last_interrupt_count = keyboard_interrupt_count;
                if (polling_mode) {
                    polling_mode = 0;
                    unsigned short *video = (unsigned short *)0xB8000;
                    video[24 * 80 + 70] = (0x0A << 8) | 'I';
                    video[24 * 80 + 71] = (0x0A << 8) | 'R';
                    video[24 * 80 + 72] = (0x0A << 8) | 'Q';
                    video[24 * 80 + 73] = (0x0A << 8) | ' ';
                }
            }
        }
        
        // Use polling if interrupts are not working
        if (polling_mode && heartbeat % 5000 == 0) {
            keyboard_poll();
        }
        
        // CPU rest
        __asm__ volatile("hlt");
    }
}
