# Detect OS
ifeq ($(OS),Windows_NT)
    EXE=win
else
    EXE=linux
endif

# Toolchain
CC      = ../../tool/gcc/$(EXE)/mips64-gcc
LD      = ../../tool/gcc/$(EXE)/mips64-ld
OBJCOPY = ../../tool/gcc/$(EXE)/mips64-objcopy
OBJDUMP = ../../tool/gcc/$(EXE)/mips64-objdump
CC1_DIR = ../../tool/gcc/$(EXE)

# Sources
SRCS    = diskC.c

# Code1's RAM address
ADDRESS = 0x80400000

# Compilation flags
CFLAGS = -G 0 -Os -I../../include/libc -I../../include -fno-toplevel-reorder --std=gnu99 -mtune=vr4300 -mabi=32 -mips3 -mno-check-zero-division -fno-builtin -mno-explicit-relocs -mno-memcpy -fno-reorder-blocks -ffast-math -fno-unsafe-math-optimizations -mno-check-zero-division -fno-optimize-sibling-calls

# Linker flags
LDFLAGS = -T entry.ld --emit-relocs

default: code1.bin

code1.bin: $(SRCS)
	@$(CC) -c diskC.c -B$(CC1_DIR) $(CFLAGS)
	@$(LD) -o diskC.elf diskC.o $(LDFLAGS)
	@$(OBJCOPY) -R .MIPS.abiflags -O binary diskC.elf diskC.bin
	@$(OBJDUMP) -t diskC.elf | grep diskC

clean:
	rm -f *.bin *.elf *.o
