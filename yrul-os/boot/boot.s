// boot/boot.s

.section .multiboot
.align 4

# Multiboot header (legacy)
.long 0x1BADB002        # Magic number for multiboot
.long 0x00000003        # Flags: align modules + memory info  
.long -(0x1BADB002 + 0x00000003)  # Checksum

# Multiboot2 header for modern QEMU
.align 8
multiboot2_start:
.long 0xe85250d6        # Magic number (multiboot2)
.long 0                 # Architecture (i386)
.long multiboot2_end - multiboot2_start  # Header length
.long -(0xe85250d6 + 0 + (multiboot2_end - multiboot2_start))  # Checksum

# End tag
.align 8
.short 0    # Type
.short 0    # Flags  
.long 8     # Size
multiboot2_end:

.section .text
.global _start
.global load_idt

_start:
    # Disable interrupts during setup
    cli
    
    # Set up a proper stack
    movl $stack_top, %esp   # Set stack pointer to our stack
    movl $stack_top, %ebp   # Set base pointer

    # Call the C kernel entry point
    call kernel_main

    # Halt the CPU if kernel_main returns
halt:
    cli                  # Clear interrupts
    hlt                  # Halt the CPU
    jmp halt             # Infinite loop to prevent exit

# Reserve 16KB for stack
.section .bss
.align 16
stack_bottom:
.skip 16384  # 16 KB stack
stack_top:

# Function to load the IDT pointer
load_idt:
    movl 4(%esp), %eax   # Load the address of the IDT pointer (passed as an argument)
    lidt (%eax)          # Load the IDT using the address in eax
    ret
