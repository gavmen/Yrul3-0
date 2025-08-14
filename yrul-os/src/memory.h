#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define PAGE_SIZE 4096
#define KERNEL_HEAP_START 0x200000
#define KERNEL_HEAP_SIZE  0x100000

typedef struct mem_block {
    uint32_t size;
    uint8_t used;
    struct mem_block *next;
} mem_block_t;

void mem_init(void);
void *kmalloc(uint32_t size);
void kfree(void *ptr);
uint32_t mem_get_free(void);

#endif
