RAM_ORIGIN  = 0x80000400;  
RAM_LENGTH  = 0x4D08;   /*  Same as BLK_SIZE_ZONE0 (the boot code can only take the one sector) */

OUTPUT_ARCH(mips)
OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")

MEMORY {
    RDRAM (rw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_LENGTH
}

SECTIONS {
    . = ALIGN(32);
    .text : ALIGN(32) 
    {
        __entry = .;
        *(EXCLUDE_FILE(*filesystem.o) .text*)
        *(.init)
        *(.fini)
    } > RDRAM

    . = ALIGN(32);
    .rodata : ALIGN(32) 
    {
        *(EXCLUDE_FILE(*filesystem.o) .rodata*)
        *(.eh_frame*)
    } > RDRAM

    . = ALIGN(32);
    .data : ALIGN(32) 
    {
        *(EXCLUDE_FILE(*filesystem.o) .data*)
        *(EXCLUDE_FILE(*filesystem.o) .sdata*)
    } > RDRAM

    . = ALIGN(32);
    .bss : ALIGN(32) 
    {
        *(EXCLUDE_FILE(*filesystem.o) COMMON)
        *(EXCLUDE_FILE(*filesystem.o) .bss*)
        *(EXCLUDE_FILE(*filesystem.o) .sbss*)
    } > RDRAM

    . = ALIGN(32);
    __ramEnd = .;
    
    __romStart = __ramEnd - RAM_ORIGIN;
    .rom __romStart : AT(__ramEnd) ALIGN(32) 
    {
        *filesystem.o(.text* .rodata* .data* .sdata* .bss* .sbss*)
    }
    __romEnd = .;    
}