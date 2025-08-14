#include "memory.h"

static mem_block_t *heap_start;
static uint32_t heap_size;

void mem_init(void) {
    heap_start = (mem_block_t *)KERNEL_HEAP_START;
    heap_size = KERNEL_HEAP_SIZE;
    
    heap_start->size = heap_size - sizeof(mem_block_t);
    heap_start->used = 0;
    heap_start->next = 0;
}

static mem_block_t *find_free_block(uint32_t size) {
    mem_block_t *current = heap_start;
    
    while (current) {
        if (!current->used && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return 0;
}

static void split_block(mem_block_t *block, uint32_t size) {
    if (block->size > size + sizeof(mem_block_t)) {
        mem_block_t *new_block = (mem_block_t *)((uint8_t *)block + sizeof(mem_block_t) + size);
        new_block->size = block->size - size - sizeof(mem_block_t);
        new_block->used = 0;
        new_block->next = block->next;
        
        block->size = size;
        block->next = new_block;
    }
}

void *kmalloc(uint32_t size) {
    if (size == 0) return 0;
    
    size = (size + 7) & ~7;
    
    mem_block_t *block = find_free_block(size);
    if (!block) return 0;
    
    split_block(block, size);
    block->used = 1;
    
    return (void *)((uint8_t *)block + sizeof(mem_block_t));
}

void kfree(void *ptr) {
    if (!ptr) return;
    
    mem_block_t *block = (mem_block_t *)((uint8_t *)ptr - sizeof(mem_block_t));
    block->used = 0;
    
    mem_block_t *current = heap_start;
    while (current && current->next) {
        if (!current->used && !current->next->used) {
            current->size += current->next->size + sizeof(mem_block_t);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

uint32_t mem_get_free(void) {
    uint32_t free_mem = 0;
    mem_block_t *current = heap_start;
    
    while (current) {
        if (!current->used) {
            free_mem += current->size;
        }
        current = current->next;
    }
    return free_mem;
}
