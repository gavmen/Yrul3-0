// src/io.c

#include "io.h"

// This file contains the implementation of I/O functions
// The functions are already implemented as inline functions in io.h
// This file can be used for additional I/O utilities if needed

void io_delay(int cycles) {
    for (int i = 0; i < cycles; i++) {
        __asm__ volatile("nop");
    }
}

