OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS
{
    . = 1M;                    /* Set start address to 1MB */

    /* Multiboot header section - MUST be first */
    .multiboot ALIGN(4) :
    {
        *(.multiboot)          /* Place the multiboot header here */
    }

    /* PVH note section for modern QEMU */
    .note.Xen ALIGN(4) :
    {
        LONG(0x0004)           /* Name size */
        LONG(0x0018)           /* Descriptor size */
        LONG(0x00000012)       /* Note type (Xen PVH) */
        BYTE(0x58) BYTE(0x65) BYTE(0x6E) BYTE(0x00) /* "Xen\0" */
        . = ALIGN(4);
        QUAD(ABSOLUTE(_start))  /* Entry point */
        QUAD(0x0)              /* Flags */
        QUAD(0x0)              /* Reserved */
    }

    /* Text section: contains code */
    .text ALIGN(4096) :
    {
        *(.text)               /* Place all .text sections here */
    }

    /* Read-only data section */
    .rodata ALIGN(4096) :
    {
        *(.rodata)             /* Place all .rodata sections here */
    }

    /* Data section: contains initialized global/static variables */
    .data ALIGN(4096) :
    {
        *(.data)               /* Place all .data sections here */
    }

    /* BSS section: contains uninitialized global/static variables */
    .bss ALIGN(4096) :
    {
        *(COMMON)              /* Place all common symbols here */
        *(.bss)                /* Place all .bss sections here */
    }
}
