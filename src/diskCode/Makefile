# Detect OS
ifeq ($(OS),Windows_NT)
    EXE=win
else
    EXE=linux
endif

CC      = ../../tool/gcc/$(EXE)/mips64-gcc
LD      = ../../tool/gcc/$(EXE)/mips64-ld
OBJCOPY = ../../tool/gcc/$(EXE)/mips64-objcopy
OBJDUMP = ../../tool/gcc/$(EXE)/mips64-objdump
CC1_DIR = ../../tool/gcc/$(EXE)

SRCS    = diskC.c filesystem.c 

ADDRESS = 0x80400000

CFLAGS = -G 0 -Os -I../../include/libc -I../../include -fno-toplevel-reorder --std=gnu99 -mtune=vr4300 -mabi=32 -mips3 -mno-check-zero-division -fno-builtin -mno-explicit-relocs -mno-memcpy -fno-reorder-blocks -ffast-math -fno-unsafe-math-optimizations -mno-check-zero-division -fno-optimize-sibling-calls

LDFLAGS = -T entry.ld --emit-relocs

default: diskC.bin

diskC.bin: diskC.o
	@$(LD) -o diskC.elf diskC.o ../yaz0Dec/yaz0.o ../filesystem/filesystem.o $(LDFLAGS)
	@$(OBJCOPY) -R .MIPS.abiflags -O binary diskC.elf diskC.bin

diskC.o: diskC.c
	@$(CC) -c diskC.c -B$(CC1_DIR) $(CFLAGS)

clean:
	rm -f *.bin *.elf *.o

