//Zelda 64 Dawn & Dusk - Expansion Disk
//By LuigiBlood

//Uses ARM9 bass

//64DD Standalone Boot Code

print "- Assemble Disk Boot Code...\n"

define IPLLOADSIZE = 10
define IPLLOADADDRESS = 0x80000400

// Code taken from krom FrameBufferCPU16BPP320x240 demo
// https://github.com/PeterLemon/N64/blob/master/FrameBuffer/16BPP/FrameBufferCPU320x240/
seekDisk0(0)
insert BOOTCODE,"../src/diskBoot/diskBoot.bin"