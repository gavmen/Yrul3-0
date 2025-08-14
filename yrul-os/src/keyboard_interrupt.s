.global keyboard_interrupt_wrapper

keyboard_interrupt_wrapper:
    pusha
    pushf
    cld
    call keyboard_handler
    popf
    popa
    iret
