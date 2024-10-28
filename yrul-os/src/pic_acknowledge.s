# src/pic_acknowledge.s
.global pic_acknowledge

# Function to acknowledge the interrupt in the PIC
pic_acknowledge:
    cmpb $0x20, %al         # Check if IRQ is >= 0x20 (IRQ 0-15)
    jb end_acknowledge      # If less than 0x20, no acknowledgment needed

    movb $0x20, %al         # Send End of Interrupt (EOI) signal to master PIC
    outb %al, $0x20

    cmpb $0x28, %al         # If IRQ is >= 0x28 (IRQ 8-15), it came from the slave PIC
    jb end_acknowledge      # If less than 0x28, no further acknowledgment needed

    movb $0x20, %al         # Send EOI signal to slave PIC
    outb %al, $0xA0

end_acknowledge:
    ret
