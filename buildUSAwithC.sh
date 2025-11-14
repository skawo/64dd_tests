#!/bin/bash
cd DiskC || { echo "Failed to enter DiskC"; exit 1; }

make clean && make

cd ..

[ -f "EZLE_DawnDusk.ndr" ] && rm "EZLE_DawnDusk.ndr"
[ -f "EZLE_DawnDusk.disk" ] && rm "EZLE_DawnDusk.disk"

./bass.exe ./asm/EZLJ_DISK_Main.asm -d USA
