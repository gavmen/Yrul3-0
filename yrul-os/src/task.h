#ifndef TASK_H
#define TASK_H

#include "types.h"

#define MAX_TASKS 64
#define KERNEL_STACK_SIZE 4096

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

typedef struct task {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi;
    uint32_t eflags;
    task_state_t state;
    uint32_t pid;
    uint8_t *stack;
    struct task *next;
} task_t;

extern task_t *current_task;
extern task_t *ready_queue;

void task_init(void);
uint32_t task_create(void (*entry_point)(void));
void task_schedule(void);
void task_yield(void);
void task_exit(void);
void task_switch(task_t *next_task);

#endif
