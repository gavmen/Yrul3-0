OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);     /* Executable and readable */
    rodata PT_LOAD FLAGS(4);   /* Read-only */
    data PT_LOAD FLAGS(6);     /* Readable and writable */
    bss PT_LOAD FLAGS(6);      /* Readable and writable */
}

SECTIONS
{
    . = 1M;                    /* Set start address to 1MB */

    /* Multiboot header section */
    .multiboot :
    {
        *(.multiboot)          /* Place the multiboot header here */
    } :text

    /* Text section: contains code */
    .text :
    {
        *(.text)               /* Place all .text sections here */
    } :text

    /* Read-only data section */
    .rodata :
    {
        *(.rodata)             /* Place all .rodata sections here */
    } :rodata

    /* Data section: contains initialized global/static variables */
    .data :
    {
        *(.data)               /* Place all .data sections here */
    } :data

    /* BSS section: contains uninitialized global/static variables */
    .bss :
    {
        *(.bss)                /* Place all .bss sections here */
        *(COMMON)              /* Place all common symbols here */
    } :bss
}
