#!/bin/sh

VERSION_PARAM="${1:-USA}"
CONVERT_FILES="${2:-n}"

OS_TYPE=$(uname -s 2>/dev/null || echo Windows)

if [ "$OS_TYPE" = "Linux" ]; then
    BASS_CMD="./bass"
    PY3_CMD="python3"
else
    BASS_CMD="./bass.exe"
    PY3_CMD="py -3"
fi

if [ "$CONVERT_FILES" = "-c" ]; then
    $PY3_CMD tool/hConv.py scene include
    $PY3_CMD tool/hConv.py object include
    $PY3_CMD tool/hConv.py images include
    $PY3_CMD tool/hConv.py other include
    $PY3_CMD tool/hConv.py audio include
fi

printf "Compiling...\n"

cd src/diskCode
make clean && make || { echo "Build failed"; exit 1; }
#$PY3_CMD ../../tool/sizes.py diskC.bin

cd ..
cd ..

printf "OK.\n\n"

rm -f *.ndr
rm -f *.disk
rm -f *.ndd

$BASS_CMD ./asm/EZLJ_DISK_Main.asm -d "$VERSION_PARAM"
