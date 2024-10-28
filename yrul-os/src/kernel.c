// src/kernel.c

// Function declarations
extern void init_idt();
extern void remap_pic();

void kernel_main(void) {
    // Initialize the PIC
    remap_pic();

    // Initialize the IDT
    init_idt();

    // Enable interrupts
    __asm__ volatile ("sti");

    // Print a message to indicate kernel start
    const char *message = "Yrul Kernel has started!\n";

    // Video memory starts at 0xB8000 for text mode
    unsigned short *video_memory = (unsigned short *)0xB8000;
    // Use white foreground and black background color for text
    unsigned char color = 0x0F; // White on black

    for (int i = 0; message[i] != '\0'; ++i) {
        // Combine the character and color attribute
        video_memory[i] = (color << 8) | message[i];
    }

    // Hang the kernel in an idle loop
    while (1) {
        __asm__ volatile("hlt");
    }
}
