// src/kernel.c

extern void init_idt();

void kernel_main(void) {
    // Initialize IDT
    init_idt();

    // Print a message to confirm that the kernel has started
    const char *message = "Yrul Kernel has started!\n";

    unsigned short *video_memory = (unsigned short *)0xB8000;
    for (int i = 0; message[i] != '\0'; ++i) {
        video_memory[i] = (video_memory[i] & 0xFF00) | message[i];
    }

    // Hang the kernel
    while (1) {
        __asm__ volatile("hlt");
    }
}
