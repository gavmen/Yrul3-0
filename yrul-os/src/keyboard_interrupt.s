// src/keyboard_interrupt_clean.s
// Clean keyboard interrupt wrapper

.global keyboard_interrupt_wrapper

keyboard_interrupt_wrapper:
    pusha                       # Save all registers
    pushf                       # Save flags
    cld                         # Clear direction flag
    
    call keyboard_handler       # Call unified handler
    
    popf                        # Restore flags
    popa                        # Restore all registers
    iret                        # Return from interrupt
