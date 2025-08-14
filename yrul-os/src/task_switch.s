.global task_switch
.global timer_interrupt_wrapper

task_switch:
    pusha
    pushf
    
    movl current_task, %eax
    testl %eax, %eax
    jz load_new_task
    
    movl %esp, (%eax)
    movl %ebp, 4(%eax)
    
load_new_task:
    movl 4(%esp), %eax
    movl %eax, current_task
    movl $1, 16(%eax)
    movl (%eax), %esp
    movl 4(%eax), %ebp
    
    popf
    popa
    ret

timer_interrupt_wrapper:
    pusha
    pushf
    cld
    call timer_handler
    call task_schedule
    popf
    popa
    iret
