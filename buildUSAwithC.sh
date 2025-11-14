#!/bin/bash
# Navigate to DiskC directory
cd DiskC || { echo "Failed to enter DiskC"; exit 1; }

# Run make clean and make
make clean && make

cd ..

# Remove files if they exist
[ -f "EZLE_DawnDusk.ndr" ] && rm "EZLE_DawnDusk.ndr"
[ -f "EZLE_DawnDusk.disk" ] && rm "EZLE_DawnDusk.disk"

# Run bass.exe via WSL (assuming it's in PATH or provide full path)
./bass.exe ./asm/EZLJ_DISK_Main.asm -d USA
