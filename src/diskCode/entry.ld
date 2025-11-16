RAM_ORIGIN  = 0x80400000; 
ROM_LENGTH = 0x3F000000;    
RAM_LENGTH  = 0x400000;      
PADDING = 0x1060;

OUTPUT_ARCH(mips)
OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")

MEMORY {
    ROM (rw) : ORIGIN = 0, LENGTH = ROM_LENGTH
    RAM (rw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_LENGTH
}

SECTIONS {
    .diskHeader : ALIGN(1) 
    {
        KEEP(*(.diskHeader))
    } > ROM   

    .diskInfo PADDING : ALIGN(1) 
    {
        KEEP(*(.diskInfo))
    } > ROM

    . = ALIGN(32);
    .rom : ALIGN(16) 
    {
        __romStart = .;
        *filesystem.o(.text* .rodata* .data* .sdata* .bss* .sbss*)
    } > ROM
    . = ALIGN(32);
    __romEnd = .;

    . = RAM_ORIGIN;
    .text : AT(__romEnd) ALIGN(4) 
    {
        __entry = .;
        *(.text*)
        *(.init)
        *(.fini)
    } > RAM
    
    . = ALIGN(32);
    .rodata : ALIGN(4) 
    {
        *(.rodata*)
        *(.eh_frame*)
    } > RAM

    . = ALIGN(32);
    .data : ALIGN(4) 
    {
        *(.data*)
        *(.sdata*)
    } > RAM

    . = ALIGN(32);
    .bss : ALIGN(4) 
    {
        *(COMMON)
        *(.bss*)
        *(.sbss*)
    } > RAM

    . = ALIGN(32);
    __ramEnd = .;

}

__Disk_Init_K1 = (0xA0000000 | Disk_Init);
__Disk_Start     = __romEnd;
__Disk_End       = __Disk_Start + __ramEnd - RAM_ORIGIN;
__Disk_VramStart = (0xA0000000 | RAM_ORIGIN);
__Disk_VramEnd   = (0xA0000000 | (RAM_ORIGIN + __Disk_End - __Disk_Start));
__ErrorIPL_Start = __romStart;