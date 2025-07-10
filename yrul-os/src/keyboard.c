// src/keyboard_unified.c
// Unified keyboard system for Yrul OS - combines all keyboard functionality

#include "keyboard.h"
#include "io.h"
#include "string.h"

// Forward declarations
static void execute_command(const char *cmd);
static void show_help(void);
static void clear_screen_area(void);
static void show_system_info(void);
static void process_command(void);
static void clear_input_line(void);
static void update_cursor(void);
static void show_test_pattern(void);
static void show_unknown_command(void);
extern void pic_acknowledge(uint8_t irq);

// Video memory and cursor management
static unsigned short *video_memory = (unsigned short *)0xB8000;
static int cursor_x = 2;  // Start after prompt ">"
static char input_buffer[80];
static int buffer_index = 0;

// Helper function to print a string to a specific line
static void print_string_to_line(const char* str, int line, uint8_t color) {
    int pos = line * 80;
    for (int i = 0; str[i] != '\0' && i < 80; i++) {
        video_memory[pos + i] = (color << 8) | str[i];
    }
}

// Interrupt tracking
volatile int keyboard_interrupt_count = 0;

// Input area configuration
#define INPUT_LINE 22
#define INPUT_START_X 2
#define INPUT_MAX_X 79

// Scancode to ASCII mapping for QEMU's translated scancode set
static const char scancode_to_ascii[128] = {
    //  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
      0,    0,   '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',    8,   '\t', // 0x00-0x0F
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',   13,   0,   'a',  's',   // 0x10-0x1F
     'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', '\'',  '`',   0,  '\\',  'z',  'x',  'c',  'v',   // 0x20-0x2F
     'b',  'n',  'm',  ',',  '.',  '/',    0,    0,    0,   ' ',   0,    0,    0,    0,    0,    0,   // 0x30-0x3F
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   // 0x40-0x4F
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   // 0x50-0x5F
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   // 0x60-0x6F
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0    // 0x70-0x7F
};

// Core keyboard functions
static uint8_t keyboard_read_status(void) {
    return inb(KEYBOARD_STATUS_PORT);
}

static uint8_t keyboard_read_data(void) {
    // Poll until data is available
    while (!(keyboard_read_status() & KEYBOARD_STATUS_OUT_FULL));
    return inb(KEYBOARD_DATA_PORT);
}

static void keyboard_write_data(uint8_t data) {
    // Poll until controller is ready for data
    while (keyboard_read_status() & KEYBOARD_STATUS_IN_FULL);
    outb(KEYBOARD_DATA_PORT, data);
}

// Display functions
static void update_cursor(void) {
    unsigned short position = INPUT_LINE * 80 + cursor_x;

    // Send the cursor position to the VGA controller
    outb(0x3D4, (unsigned char)((position >> 8) & 0xFF));
    outb(0x3D5, (unsigned char)(position & 0xFF));
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

// Command history management
static char last_command[80];
static int command_display_timer = 0;
#define COMMAND_DISPLAY_TIME 2000  // Show command for ~2000 cycles (more persistent)

static void process_command(void) {
    input_buffer[buffer_index] = '\0';
    
    // Store command in history
    for (int i = 0; i < 80 && i < buffer_index; i++) {
        last_command[i] = input_buffer[i];
    }
    last_command[buffer_index] = '\0';
    
    // Show processed command on line 23 with persistence
    for (int i = 0; i < 80; i++) {
        video_memory[23 * 80 + i] = (0x07 << 8) | ' ';
    }
    
    const char *prefix = "CMD: ";
    for (int i = 0; prefix[i] && i < 5; i++) {
        video_memory[23 * 80 + i] = (0x0E << 8) | prefix[i];
    }
    
    // Display the command in green
    for (int i = 0; last_command[i] && i < 70; i++) {
        video_memory[23 * 80 + 5 + i] = (0x0A << 8) | last_command[i];
    }
    
    // Set timer for command persistence
    command_display_timer = COMMAND_DISPLAY_TIME;
    
    // Execute basic commands
    if (buffer_index > 0) {
        execute_command(input_buffer);
    }
    
    clear_input_line();
}

// Basic command execution - ROBUST
static void execute_command(const char *cmd) {
    // Show execution indicator
    video_memory[74] = (0x0C << 8) | 'E';
    video_memory[75] = (0x0C << 8) | 'X';
    video_memory[76] = (0x0C << 8) | 'E';
    
    // Use strcmp for robust command matching
    if (strcmp(cmd, "help") == 0) {
        show_help();
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen_area();
    } else if (strcmp(cmd, "info") == 0) {
        show_system_info();
    } else if (strcmp(cmd, "test") == 0) {
        show_test_pattern();
    } else {
        show_unknown_command();
    }
}

// Command implementations
static void clear_command_output_area(void) {
    for (int line = 15; line <= 21; line++) {
        for (int col = 0; col < 80; col++) {
            video_memory[line * 80 + col] = (0x0F << 8) | ' ';
        }
    }
}

static void show_help(void) {
    clear_command_output_area();
    print_string_to_line("YRUL OS COMMANDS:", 15, 0x0E);
    print_string_to_line("help  - Show this help", 16, 0x0F);
    print_string_to_line("clear - Clear command area", 17, 0x0F);
    print_string_to_line("info  - System information", 18, 0x0F);
    print_string_to_line("test  - Display test pattern", 19, 0x0F);
    print_string_to_line("Use Backspace to edit, Enter to execute", 21, 0x0F);
}

// Clear command area
static void clear_screen_area(void) {
    clear_command_output_area();
}

// System info - IMPROVED
static void show_system_info(void) {
    clear_command_output_area();
    print_string_to_line("SYSTEM INFORMATION:", 15, 0x0E);
    print_string_to_line("OS: Yrul OS v1.2", 16, 0x0B);
    print_string_to_line("Architecture: x86 32-bit", 17, 0x0B);
    print_string_to_line("Memory: VGA Text Mode (0xB8000)", 18, 0x0B);
    print_string_to_line("Keyboard: PS/2 Controller", 19, 0x0B);
    print_string_to_line("Status: Running in interrupt mode", 20, 0x0B);

    // Show interrupt count
    int interrupts = keyboard_interrupt_count;
    int pos = 21 * 80;
    const char *prefix = "Interrupts: ";
    for (int i = 0; prefix[i]; i++) {
        video_memory[pos + i] = (0x0A << 8) | prefix[i];
    }
    
    // Simple number to string conversion
    int digit_pos = pos + 12;
    if (interrupts == 0) {
        video_memory[digit_pos] = (0x0A << 8) | '0';
    } else {
        int temp = interrupts;
        int digits = 0;
        while (temp > 0) {
            temp /= 10;
            digits++;
        }
        for (int i = digits - 1; i >= 0; i--) {
            video_memory[digit_pos + i] = (0x0A << 8) | ('0' + (interrupts % 10));
            interrupts /= 10;
        }
    }
}

// Keyboard initialization - SIMPLIFIED
void keyboard_init(void) {
    // Flush the keyboard buffer thoroughly
    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUT_FULL) {
        inb(KEYBOARD_DATA_PORT);
    }
}

// Main keyboard interrupt handler - CLEANED
void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    keyboard_interrupt_count++;

    // Ignore key release events (top bit set) and special codes
    if (scancode > 0x80 || scancode == 0xFA || scancode == 0xFE) {
        pic_acknowledge(1);
        return;
    }

    char ascii_char = scancode_to_ascii[scancode];

    if (ascii_char) {
        if (ascii_char == 13) { // Enter key
            process_command();
        } else if (ascii_char == 8) { // Backspace
            if (buffer_index > 0) {
                buffer_index--;
                cursor_x--;
                video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ' ';
            }
        } else { // Printable character
            if (buffer_index < sizeof(input_buffer) - 1 && cursor_x < INPUT_MAX_X) {
                input_buffer[buffer_index++] = ascii_char;
                video_memory[INPUT_LINE * 80 + cursor_x++] = (0x0F << 8) | ascii_char;
            }
        }
    }

    update_cursor();
    pic_acknowledge(1); // Acknowledge interrupt at the end
}

// Polling fallback is no longer needed and has been removed.

// Initialize input area
void keyboard_setup_display(void) {
    // Set up prompt on input line
    video_memory[INPUT_LINE * 80] = (0x0A << 8) | '>';
    video_memory[INPUT_LINE * 80 + 1] = (0x0A << 8) | ' ';
    clear_input_line();
}

// New command functions
static void show_test_pattern(void) {
    clear_command_output_area();
    // Display a colorful test pattern
    for (int i = 0; i < 60; i++) {
        int color = (i % 7) + 1; // Cycle through colors 1-7
        video_memory[20 * 80 + i] = (color << 8) | ('A' + (i % 26));
    }
}

static void show_unknown_command(void) {
    clear_command_output_area();
    print_string_to_line("Unknown command. Type 'help' for a list of commands.", 21, 0x0C);
}
