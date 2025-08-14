#include "io.h"

void io_delay(int cycles) {
    for (int i = 0; i < cycles; i++) {
        __asm__ volatile("nop");
    }
}

