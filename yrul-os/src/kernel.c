// src/kernel.c

void kernel_main(void) {
    const char *message = "Hello, Yrul!";
    char *video_memory = (char *) 0xb8000;
    for (int i = 0; message[i] != '\0'; i++) {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x07; // Light grey on black background
    }
}
