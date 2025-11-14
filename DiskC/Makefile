CC      = mips64-gcc
LD      = mips64-ld
OBJCOPY = mips64-objcopy
OBJDUMP = mips64-objdump
SRCS    = code1.c

# code1's ram address
ADDRESS = 0x80400000

# default compilation flags
CFLAGS = -G 0 -Os -Iinclude/libc -Iinclude -fno-toplevel-reorder --std=gnu99 -mtune=vr4300 -mabi=32 -mips3 -mno-check-zero-division -fno-builtin -mno-explicit-relocs -mno-memcpy -fno-reorder-blocks -ffast-math -fno-unsafe-math-optimizations -mno-check-zero-division -fno-optimize-sibling-calls

LDFLAGS = --emit-relocs -T
LDFLAGS = -T entry.ld --emit-relocs

default: code1.bin

code1.bin: $(SRCS)
	@$(CC) -c code1.c $(CFLAGS)
	@$(LD) -o code1.elf code1.o $(LDFLAGS) $(LDFILE)
	@$(OBJCOPY) -R .MIPS.abiflags -O binary code1.elf code1.bin
	@$(OBJDUMP) -t code1.elf | grep code1

clean:
	rm -f *.bin *.elf *.o