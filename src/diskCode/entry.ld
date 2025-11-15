RAM_ORIGIN  = 0x80400000;  
RAM_LENGTH  = 0x400000;      
DISKINFO_ADDR = RAM_ORIGIN + 0x1060;

OUTPUT_ARCH(mips)
OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")

MEMORY {
    RDRAM (rw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_LENGTH
}

SECTIONS {
    .ddHookTable RAM_ORIGIN : ALIGN(32) 
    {
        KEEP(*(.ddHookTable))
    } > RDRAM

    . = ALIGN(32);
    .variableRAM : ALIGN(32) 
    {
        KEEP(*(.variableRAM))
    } > RDRAM

    .diskInfo DISKINFO_ADDR : ALIGN(32) 
    {
        KEEP(*(.diskInfo))
    } > RDRAM

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

__Disk_Init_K1 = (0xA0000000 | Disk_Init);
__Disk_Start     = 0;
__Disk_End       = __ramEnd - RAM_ORIGIN;
__Disk_VramStart = (0xA0000000 | RAM_ORIGIN);
__Disk_VramEnd   = (0xA0000000 | (RAM_ORIGIN + __Disk_End));
