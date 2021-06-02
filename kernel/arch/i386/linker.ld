ENTRY (_start)

SECTIONS
{
    /* Base address starting at 1MB */
    . = 0x00100000;
    EARLY_KERNEL_START = .;

    /* Special multiboot compliant header.
     * Used by GRUB for image detection.
     */
    .multiboot_header : { *(.multiboot_header) }
    .early_text ALIGN (0x1000) : { *(.early_text) }
    .early_data ALIGN (0x1000) : { *(.early_data) }
    .early_bss ALIGN (0x1000) :
    {
        EARLY_BSS_START = .;
        *(.early_bss)
        EARLY_BSS_END = .;
    }

    EARLY_KERNEL_END = .;

    /* Higher half (3 GB) */
    . += 0xC0000000;

    /* Let the source know about the kernel start */
    KERNEL_START = .;

    /* make sure the sections are page aligned */
    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000) { *(.text) }
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xC0000000) { *(.rodata*) }
    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) { *(.data) }
    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xC0000000)
    {
        BSS_START = .;
        *(.bss)
        *(COMMON)
        BSS_END = .;
    }
    .early_kmalloc ALIGN (0x1000) : AT(ADDR(.early_kmalloc) - 0xC0000000)
    {
        EARLY_KMALLOC_START = .;
        . += 0x100000; /* 1MB of heap space */
        EARLY_KMALLOC_END = .;
    }

    /* Let the source know about the kernel end */
    KERNEL_END = .;
    EARLY_BSS_SIZE = EARLY_BSS_END - EARLY_BSS_START;
    BSS_SIZE = BSS_END - BSS_START;
    KERNEL_SIZE = KERNEL_END - KERNEL_START;
}

ASSERT(KERNEL_SIZE < 0x300000, "Kernel exceeds the 3 MB limit. kernel/arch/i386/boot.s must be updated to work with a >3MB kernel");