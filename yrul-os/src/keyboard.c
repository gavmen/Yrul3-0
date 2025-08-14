#include "keyboard.h"
#include "io.h"
#include "string.h"
#include "memory.h"
#include "timer.h"
#include "task.h"

static void execute_command(const char *cmd);
static void show_help(void);
static void clear_screen_area(void);
static void show_system_info(void);
static void process_command(void);
static void clear_input_line(void);
static void update_cursor(void);
static void show_test_pattern(void);
static void show_unknown_command(void);
static void show_memory_info(void);
static void show_process_info(void);
extern void pic_acknowledge(uint8_t irq);

static unsigned short *video_memory = (unsigned short *)0xB8000;
static int cursor_x = 2;
static char input_buffer[80];
static int buffer_index = 0;

static void print_string_to_line(const char* str, int line, uint8_t color) {
    if (line < 0 || line >= 25 || !str) return;
    
    int pos = line * 80;
    for (int i = 0; str[i] != '\0' && i < 80; i++) {
        video_memory[pos + i] = (color << 8) | str[i];
    }
}

volatile int keyboard_interrupt_count = 0;

#define INPUT_LINE 22
#define INPUT_START_X 2
#define INPUT_MAX_X 79
static const char scancode_to_ascii[128] = {
      0,    0,   '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',    8,   '\t',
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',   13,   0,   'a',  's',
     'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', '\'',  '`',   0,  '\\',  'z',  'x',  'c',  'v',
     'b',  'n',  'm',  ',',  '.',  '/',    0,    0,    0,   ' ',   0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
};

static uint8_t keyboard_read_status(void) {
    return inb(KEYBOARD_STATUS_PORT);
}

static uint8_t keyboard_read_data(void) __attribute__((unused));
static uint8_t keyboard_read_data(void) {
    while (!(keyboard_read_status() & KEYBOARD_STATUS_OUT_FULL));
    return inb(KEYBOARD_DATA_PORT);
}

static void keyboard_write_data(uint8_t data) __attribute__((unused));
static void keyboard_write_data(uint8_t data) {
    while (keyboard_read_status() & KEYBOARD_STATUS_IN_FULL);
    outb(KEYBOARD_DATA_PORT, data);
}

static void update_cursor(void) {
    if (cursor_x < 0) cursor_x = INPUT_START_X;
    if (cursor_x > 79) cursor_x = 79;
    
    unsigned short position = INPUT_LINE * 80 + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

static void clear_input_line(void) {
    for (int i = INPUT_START_X; i < 80; i++) {
        video_memory[INPUT_LINE * 80 + i] = (0x0F << 8) | ' ';
    }
    cursor_x = INPUT_START_X;
    buffer_index = 0;
    for (int i = 0; i < (int)sizeof(input_buffer); i++) {
        input_buffer[i] = 0;
    }
    update_cursor();
}

static char last_command[80];
static int command_display_timer = 0;
#define COMMAND_DISPLAY_TIME 2000

static void process_command(void) {
    input_buffer[buffer_index] = '\0';
    
    for (int i = 0; i < 80 && i < buffer_index; i++) {
        last_command[i] = input_buffer[i];
    }
    if (buffer_index < 80) {
        last_command[buffer_index] = '\0';
    } else {
        last_command[79] = '\0';
    }
    
    for (int i = 0; i < 80; i++) {
        video_memory[23 * 80 + i] = (0x07 << 8) | ' ';
    }
    
    const char *prefix = "CMD: ";
    for (int i = 0; prefix[i] && i < 5; i++) {
        video_memory[23 * 80 + i] = (0x0E << 8) | prefix[i];
    }
    
    for (int i = 0; last_command[i] && i < 70; i++) {
        video_memory[23 * 80 + 5 + i] = (0x0A << 8) | last_command[i];
    }
    
    command_display_timer = COMMAND_DISPLAY_TIME;
    
    if (buffer_index > 0) {
        execute_command(input_buffer);
    }
    
    clear_input_line();
}

static void execute_command(const char *cmd) {
    video_memory[74] = (0x0C << 8) | 'E';
    video_memory[75] = (0x0C << 8) | 'X';
    video_memory[76] = (0x0C << 8) | 'E';
    
    if (strcmp(cmd, "help") == 0) {
        show_help();
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen_area();
    } else if (strcmp(cmd, "info") == 0) {
        show_system_info();
    } else if (strcmp(cmd, "test") == 0) {
        show_test_pattern();
    } else if (strcmp(cmd, "ver") == 0) {
        show_system_info();
    } else if (strcmp(cmd, "mem") == 0) {
        show_memory_info();
    } else if (strcmp(cmd, "ps") == 0) {
        show_process_info();
    } else {
        show_unknown_command();
    }
}

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
    print_string_to_line("mem   - Memory statistics", 20, 0x0F);
    print_string_to_line("ps    - Process information", 21, 0x0F);
}

static void clear_screen_area(void) {
    clear_command_output_area();
}

static void show_system_info(void) {
    clear_command_output_area();
    print_string_to_line("SYSTEM INFORMATION:", 15, 0x0E);
    print_string_to_line("OS: Yrul OS v1.2", 16, 0x0B);
    print_string_to_line("Architecture: x86 32-bit", 17, 0x0B);
    print_string_to_line("Memory: VGA Text Mode (0xB8000)", 18, 0x0B);
    print_string_to_line("Keyboard: PS/2 Controller", 19, 0x0B);
    print_string_to_line("Status: Running in interrupt mode", 20, 0x0B);

    int interrupts = keyboard_interrupt_count;
    int pos = 21 * 80;
    const char *prefix = "Interrupts: ";
    for (int i = 0; prefix[i]; i++) {
        video_memory[pos + i] = (0x0A << 8) | prefix[i];
    }
    
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
        
        if (digits > 8) digits = 8;
        
        temp = interrupts;
        for (int i = digits - 1; i >= 0; i--) {
            video_memory[digit_pos + i] = (0x0A << 8) | ('0' + (temp % 10));
            temp /= 10;
        }
    }
}

void keyboard_init(void) {
    int timeout = 1000;
    while ((inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUT_FULL) && timeout--) {
        inb(KEYBOARD_DATA_PORT);
        for (volatile int i = 0; i < 1000; i++);
    }
}

void keyboard_handler(void) {
    uint8_t status = inb(KEYBOARD_STATUS_PORT);
    if (!(status & KEYBOARD_STATUS_OUT_FULL)) {
        pic_acknowledge(1);
        return;
    }
    
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    keyboard_interrupt_count++;

    if (scancode > 0x80 || scancode == 0xFA || scancode == 0xFE) {
        pic_acknowledge(1);
        return;
    }

    char ascii_char = scancode_to_ascii[scancode];

    if (ascii_char) {
        if (ascii_char == 13) {
            process_command();
        } else if (ascii_char == 8) {
            if (buffer_index > 0 && cursor_x > INPUT_START_X) {
                buffer_index--;
                cursor_x--;
                video_memory[INPUT_LINE * 80 + cursor_x] = (0x0F << 8) | ' ';
            }
        } else {
            if (buffer_index < (int)(sizeof(input_buffer) - 1) && cursor_x < INPUT_MAX_X) {
                input_buffer[buffer_index++] = ascii_char;
                video_memory[INPUT_LINE * 80 + cursor_x++] = (0x0F << 8) | ascii_char;
            }
        }
    }

    update_cursor();
    pic_acknowledge(1);
}

void keyboard_setup_display(void) {
    video_memory[INPUT_LINE * 80] = (0x0A << 8) | '>';
    video_memory[INPUT_LINE * 80 + 1] = (0x0A << 8) | ' ';
    clear_input_line();
}

static void show_test_pattern(void) {
    clear_command_output_area();
    for (int i = 0; i < 60; i++) {
        int color = (i % 7) + 1;
        video_memory[20 * 80 + i] = (color << 8) | ('A' + (i % 26));
    }
}

static void show_unknown_command(void) {
    clear_command_output_area();
    print_string_to_line("Unknown command. Type 'help' for a list of commands.", 21, 0x0C);
}

static void show_memory_info(void) {
    clear_command_output_area();
    print_string_to_line("MEMORY INFORMATION:", 15, 0x0E);
    print_string_to_line("Heap Start: 0x200000", 16, 0x0B);
    print_string_to_line("Heap Size:  1MB", 17, 0x0B);
    
    uint32_t free_mem = mem_get_free();
    int pos = 18 * 80;
    const char *prefix = "Free Memory: ";
    for (int i = 0; prefix[i]; i++) {
        video_memory[pos + i] = (0x0A << 8) | prefix[i];
    }
    
    int digit_pos = pos + 13;
    if (free_mem == 0) {
        video_memory[digit_pos] = (0x0A << 8) | '0';
    } else {
        int temp = free_mem;
        int digits = 0;
        while (temp > 0) {
            temp /= 10;
            digits++;
        }
        if (digits > 8) digits = 8;
        temp = free_mem;
        for (int i = digits - 1; i >= 0; i--) {
            video_memory[digit_pos + i] = (0x0A << 8) | ('0' + (temp % 10));
            temp /= 10;
        }
        video_memory[digit_pos + digits] = (0x0A << 8) | ' ';
        video_memory[digit_pos + digits + 1] = (0x0A << 8) | 'b';
        video_memory[digit_pos + digits + 2] = (0x0A << 8) | 'y';
        video_memory[digit_pos + digits + 3] = (0x0A << 8) | 't';
        video_memory[digit_pos + digits + 4] = (0x0A << 8) | 'e';
        video_memory[digit_pos + digits + 5] = (0x0A << 8) | 's';
    }
}

static void show_process_info(void) {
    clear_command_output_area();
    print_string_to_line("PROCESS INFORMATION:", 15, 0x0E);
    
    uint32_t ticks = timer_get_ticks();
    int pos = 16 * 80;
    const char *prefix = "System uptime: ";
    for (int i = 0; prefix[i]; i++) {
        video_memory[pos + i] = (0x0B << 8) | prefix[i];
    }
    
    int digit_pos = pos + 15;
    if (ticks == 0) {
        video_memory[digit_pos] = (0x0B << 8) | '0';
    } else {
        int temp = ticks;
        int digits = 0;
        while (temp > 0) {
            temp /= 10;
            digits++;
        }
        if (digits > 8) digits = 8;
        temp = ticks;
        for (int i = digits - 1; i >= 0; i--) {
            video_memory[digit_pos + i] = (0x0B << 8) | ('0' + (temp % 10));
            temp /= 10;
        }
        video_memory[digit_pos + digits] = (0x0B << 8) | ' ';
        video_memory[digit_pos + digits + 1] = (0x0B << 8) | 't';
        video_memory[digit_pos + digits + 2] = (0x0B << 8) | 'i';
        video_memory[digit_pos + digits + 3] = (0x0B << 8) | 'c';
        video_memory[digit_pos + digits + 4] = (0x0B << 8) | 'k';
        video_memory[digit_pos + digits + 5] = (0x0B << 8) | 's';
    }
    
    print_string_to_line("Scheduler: Preemptive multitasking", 17, 0x0B);
    print_string_to_line("Timer frequency: 100 Hz", 18, 0x0B);
    
    if (current_task) {
        pos = 19 * 80;
        const char *prefix2 = "Current PID: ";
        for (int i = 0; prefix2[i]; i++) {
            video_memory[pos + i] = (0x0A << 8) | prefix2[i];
        }
        
        uint32_t pid = current_task->pid;
        digit_pos = pos + 13;
        if (pid == 0) {
            video_memory[digit_pos] = (0x0A << 8) | '0';
        } else {
            int temp = pid;
            int digits = 0;
            while (temp > 0) {
                temp /= 10;
                digits++;
            }
            temp = pid;
            for (int i = digits - 1; i >= 0; i--) {
                video_memory[digit_pos + i] = (0x0A << 8) | ('0' + (temp % 10));
                temp /= 10;
            }
        }
    } else {
        print_string_to_line("No active processes", 19, 0x0C);
    }
}
