#include "timer.h"
#include "io.h"
#include "interrupts.h"

static volatile uint32_t timer_ticks = 0;

void timer_init(void) {
    uint32_t divisor = 1193180 / TIMER_FREQ;
    
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void timer_handler(void) {
    timer_ticks++;
    pic_acknowledge(TIMER_IRQ);
}

uint32_t timer_get_ticks(void) {
    return timer_ticks;
}
