#include "task.h"
#include "memory.h"
#include "string.h"

task_t *current_task = 0;
task_t *ready_queue = 0;
static uint32_t next_pid = 1;
static task_t task_pool[MAX_TASKS];
static uint8_t task_initialized = 0;

void task_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        task_pool[i].state = TASK_TERMINATED;
        task_pool[i].pid = 0;
        task_pool[i].next = 0;
        task_pool[i].stack = 0;
    }
    task_initialized = 1;
}

static task_t *allocate_task(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_pool[i].state == TASK_TERMINATED) {
            return &task_pool[i];
        }
    }
    return 0;
}

uint32_t task_create(void (*entry_point)(void)) {
    if (!task_initialized) return 0;
    
    task_t *task = allocate_task();
    if (!task) return 0;
    
    task->stack = (uint8_t *)kmalloc(KERNEL_STACK_SIZE);
    if (!task->stack) return 0;
    
    task->pid = next_pid++;
    task->state = TASK_READY;
    task->esp = (uint32_t)(task->stack + KERNEL_STACK_SIZE - 4);
    task->ebp = task->esp;
    task->eip = (uint32_t)entry_point;
    task->eflags = 0x202;
    task->eax = task->ebx = task->ecx = task->edx = 0;
    task->esi = task->edi = 0;
    task->next = 0;
    
    if (!ready_queue) {
        ready_queue = task;
        task->next = task;
    } else {
        task->next = ready_queue->next;
        ready_queue->next = task;
    }
    
    return task->pid;
}

void task_schedule(void) {
    if (!ready_queue) return;
    
    task_t *next_task = ready_queue->next;
    if (next_task && next_task != current_task) {
        ready_queue = next_task;
        task_switch(next_task);
    }
}

void task_yield(void) {
    if (current_task) {
        current_task->state = TASK_READY;
    }
    task_schedule();
}

void task_exit(void) {
    if (!current_task) return;
    
    current_task->state = TASK_TERMINATED;
    if (current_task->stack) {
        kfree(current_task->stack);
        current_task->stack = 0;
    }
    
    if (ready_queue == current_task) {
        if (current_task->next == current_task) {
            ready_queue = 0;
        } else {
            task_t *temp = current_task;
            while (temp->next != current_task) {
                temp = temp->next;
            }
            temp->next = current_task->next;
            ready_queue = current_task->next;
        }
    }
    
    current_task = 0;
    task_schedule();
}
