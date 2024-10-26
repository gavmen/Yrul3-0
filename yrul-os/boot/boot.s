.section .multiboot
.align 4
.long 0x1BADB002        # magic number
.long 0x00              # flags
.long -(0x1BADB002)     # checksum

.section .text
.global _start
_start:
    # Call the C kernel entry point
    call kernel_main

    # Halt the CPU if kernel_main returns
halt:
    cli                 # Clear interrupts
    hlt                 # Halt the CPU
    jmp halt            # Infinite loop to prevent exit
