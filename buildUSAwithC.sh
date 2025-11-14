#!/bin/sh
py3command=false

if command -v py >/dev/null 2>&1; then 
    py3command=true
else
    py3command=false
fi


cd src || { echo "Failed to enter src"; exit 1; }

make clean && make

if [ "$py3command" = true ]; then
    py -3 sizes.py;
else
    python3 sizes.py;
fi

cd ..

[ -f "EZLE_DawnDusk.ndr" ] && rm "EZLE_DawnDusk.ndr"
[ -f "EZLE_DawnDusk.disk" ] && rm "EZLE_DawnDusk.disk"

./bass.exe ./asm/EZLJ_DISK_Main.asm -d USA
