// src/pic_acknowledge.s
.global pic_acknowledge

// Function to acknowledge the interrupt in the PIC
pic_acknowledge:
    movl 4(%esp), %eax     # Get the IRQ number from the stack (first parameter)
    cmpb $0x28, %al        # Check if the IRQ number >= 0x28 (IRQ 8-15)
    jb master_eoi          # If IRQ is from master PIC, only send to PIC1

    movb $0x20, %al        # Send EOI to slave PIC (0xA0)
    outb %al, $0xA0

master_eoi:
    movb $0x20, %al        # Send EOI to master PIC (0x20)
    outb %al, $0x20
    ret
