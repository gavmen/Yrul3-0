// src/keyboard_unified.c
// Unified keyboard system for Yrul OS - combines all keyboard functionality

#include "keyboard.h"
#include "io.h"

// Video memory and cursor management
static unsigned short *video_memory = (unsigned short *)0xB8000;
static int cursor_x = 2;  // Start after prompt ">"
static int cursor_y = 22; // Input line
static char input_buffer[80];
static int buffer_index = 0;

// Interrupt tracking
volatile int keyboard_interrupt_count = 0;

// Input area configuration
#define INPUT_LINE 22
#define INPUT_START_X 2
#define INPUT_MAX_X 79

// Complete scancode to ASCII mapping
static const char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',    // 0-9
    '9', '0', '-', '=', 8,   9,                       // 10-15: Backspace, Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', // 16-25
    '[', ']', 13,                                     // 26-28: Enter
    0,                                                // 29: Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 30-39
    '\'', '`', 0, '\\',                               // 40-43: Left shift
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', // 44-53
    0, '*', 0, ' ',                                   // 54-57: Right shift, Alt, Space
    [58 ... 127] = 0                                  // Rest as zeros
};

// Core keyboard functions
static void keyboard_wait(void) {
    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_IN_FULL);
}

static uint8_t keyboard_read_data(void) {
    return inb(KEYBOARD_DATA_PORT);
}

static uint8_t keyboard_read_status(void) {
    return inb(KEYBOARD_STATUS_PORT);
}

static void keyboard_write_cmd(uint8_t cmd) {
    keyboard_wait();
    outb(KEYBOARD_COMMAND_PORT, cmd);
}

static void keyboard_write_data(uint8_t data) {
    keyboard_wait();
    outb(KEYBOARD_DATA_PORT, data);
}

// Display functions
static void update_cursor(void) {
    // Clear old cursor
    for (int i = INPUT_START_X; i < INPUT_MAX_X; i++) {
        if ((video_memory[INPUT_LINE * 80 + i] & 0xFF) == '_') {
            video_memory[INPUT_LINE * 80 + i] = (0x0F << 8) | ' ';
        }
    }
    // Show cursor at current position
    if (cursor_x < INPUT_MAX_X) {
        video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | '_';
    }
}

static void clear_input_line(void) {
    for (int i = INPUT_START_X; i < 80; i++) {
        video_memory[INPUT_LINE * 80 + i] = (0x0F << 8) | ' ';
    }
    cursor_x = INPUT_START_X;
    buffer_index = 0;
    for (int i = 0; i < 80; i++) {
        input_buffer[i] = 0;
    }
    update_cursor();
}

static void process_command(void) {
    input_buffer[buffer_index] = '\0';
    
    // Show processed command on line 23
    for (int i = 0; i < 80; i++) {
        video_memory[23 * 80 + i] = (0x07 << 8) | ' ';
    }
    
    const char *prefix = "CMD: ";
    for (int i = 0; prefix[i] && i < 5; i++) {
        video_memory[23 * 80 + i] = (0x0E << 8) | prefix[i];
    }
    
    for (int i = 0; input_buffer[i] && i < 70; i++) {
        video_memory[23 * 80 + 5 + i] = (0x0A << 8) | input_buffer[i];
    }
    
    clear_input_line();
}

// Keyboard initialization
void keyboard_init(void) {
    // Disable PS/2 port
    keyboard_write_cmd(KEYBOARD_CMD_DISABLE_PS2);
    
    // Flush output buffer
    while (keyboard_read_status() & KEYBOARD_STATUS_OUT_FULL) {
        keyboard_read_data();
    }
    
    // Configure controller
    keyboard_write_cmd(KEYBOARD_CMD_READ_CONFIG);
    while (!(keyboard_read_status() & KEYBOARD_STATUS_OUT_FULL));
    uint8_t config = keyboard_read_data();
    
    config |= 0x01;   // Enable interrupt
    config &= ~0x40;  // Disable translation
    
    keyboard_write_cmd(KEYBOARD_CMD_WRITE_CONFIG);
    keyboard_write_data(config);
    
    // Enable PS/2 port
    keyboard_write_cmd(KEYBOARD_CMD_ENABLE_PS2);
    
    // Reset keyboard
    keyboard_write_data(KEYBOARD_CMD_RESET);
    
    // Wait for reset completion
    int timeout = 10000;
    while (timeout-- > 0) {
        if (keyboard_read_status() & KEYBOARD_STATUS_OUT_FULL) {
            uint8_t response = keyboard_read_data();
            if (response == 0xAA) break; // Self-test passed
        }
    }
}

// Main keyboard interrupt handler
void keyboard_handler(void) {
    keyboard_interrupt_count++;
    
    // Show interrupt count (debug indicator)
    video_memory[79] = (0x0C << 8) | ('0' + (keyboard_interrupt_count % 10));
    
    uint8_t status = keyboard_read_status();
    
    if (status & KEYBOARD_STATUS_OUT_FULL) {
        uint8_t scancode = keyboard_read_data();
        
        // Show scancode in hex (debug)
        static const char hex[] = "0123456789ABCDEF";
        video_memory[77] = (0x0E << 8) | hex[(scancode >> 4) & 0xF];
        video_memory[78] = (0x0E << 8) | hex[scancode & 0xF];
        
        // Process only key press (not release)
        if (!(scancode & 0x80)) {
            char ascii = 0;
            if (scancode < 128) {
                ascii = scancode_to_ascii[scancode];
            }
            
            if (ascii != 0) {
                if (ascii == 13 || ascii == '\n') { // Enter
                    process_command();
                } else if (ascii == 8) { // Backspace
                    if (cursor_x > INPUT_START_X && buffer_index > 0) {
                        cursor_x--;
                        buffer_index--;
                        video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ' ';
                        input_buffer[buffer_index] = 0;
                        update_cursor();
                    }
                } else if (ascii >= 32 && ascii <= 126) { // Printable characters
                    if (cursor_x < INPUT_MAX_X && buffer_index < 79) {
                        video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ascii;
                        input_buffer[buffer_index] = ascii;
                        buffer_index++;
                        cursor_x++;
                        update_cursor();
                    }
                }
            }
        }
    }
    
    // Acknowledge interrupt
    extern void pic_acknowledge(uint8_t irq);
    pic_acknowledge(1);
}

// Polling fallback (if interrupts fail)
void keyboard_poll(void) {
    static int poll_count = 0;
    poll_count++;
    
    // Show polling indicator
    video_memory[76] = (0x0C << 8) | 'P';
    video_memory[75] = (0x0C << 8) | ('0' + (poll_count % 10));
    
    uint8_t status = keyboard_read_status();
    
    if (status & KEYBOARD_STATUS_OUT_FULL) {
        uint8_t scancode = keyboard_read_data();
        
        if (!(scancode & 0x80)) { // Key press only
            char ascii = 0;
            if (scancode < 128) {
                ascii = scancode_to_ascii[scancode];
            }
            
            if (ascii != 0) {
                if (ascii == 13) {
                    process_command();
                } else if (ascii == 8) {
                    if (cursor_x > INPUT_START_X && buffer_index > 0) {
                        cursor_x--;
                        buffer_index--;
                        video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ' ';
                        input_buffer[buffer_index] = 0;
                        update_cursor();
                    }
                } else if (ascii >= 32 && ascii <= 126) {
                    if (cursor_x < INPUT_MAX_X && buffer_index < 79) {
                        video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ascii;
                        input_buffer[buffer_index] = ascii;
                        buffer_index++;
                        cursor_x++;
                        update_cursor();
                    }
                }
            }
        }
    }
}

// Initialize input area
void keyboard_setup_display(void) {
    // Set up prompt on input line
    video_memory[INPUT_LINE * 80] = (0x0A << 8) | '>';
    video_memory[INPUT_LINE * 80 + 1] = (0x0A << 8) | ' ';
    clear_input_line();
}
