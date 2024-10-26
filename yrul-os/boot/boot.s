// boot/boot.s

.section .multiboot
.align 4
.long 0x1BADB002        # Magic number for multiboot
.long 0x00              # Flags
.long -(0x1BADB002)     # Checksum

.section .text
.global _start
.global load_idt

_start:
    # Set up a stack
    movl $0x9FC00, %esp  # Set stack pointer to 0x9FC00 (just below 1MB)
    movl $0x9FC00, %ebp  # Set base pointer

    # Call the C kernel entry point
    call kernel_main

    # Halt the CPU if kernel_main returns
halt:
    cli                  # Clear interrupts
    hlt                  # Halt the CPU
    jmp halt             # Infinite loop to prevent exit

# Load the IDT pointer
#load_idt:
#    movl 4(%esp), %eax   # Load the first argument (address of IDT pointer) into eax
#    lidt (%eax)          # Load the IDT using the address in eax
#    ret

.section .text
.global load_idt

load_idt:
    lidt (%esp)
    ret
