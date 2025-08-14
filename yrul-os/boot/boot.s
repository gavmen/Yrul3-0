.section .multiboot
.align 4

.long 0x1BADB002
.long 0x00000003
.long -(0x1BADB002 + 0x00000003)

.align 8
multiboot2_start:
.long 0xe85250d6
.long 0
.long multiboot2_end - multiboot2_start
.long -(0xe85250d6 + 0 + (multiboot2_end - multiboot2_start))

.align 8
.short 0
.short 0
.long 8
multiboot2_end:

.section .text
.global _start
.global load_idt

_start:
    cli
    movl $stack_top, %esp
    movl $stack_top, %ebp
    call kernel_main

halt:
    cli
    hlt
    jmp halt

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

load_idt:
    movl 4(%esp), %eax
    lidt (%eax)
    ret
