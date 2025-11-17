#!/bin/sh

VERSION_PARAM="${1:-USA}"
CONVERT_FILES="${2:-n}"
CLEAN="${3:-ncl}"

OS_TYPE=$(uname -s 2>/dev/null || echo Windows)

if [ "$OS_TYPE" = "Linux" ]; then
    BASS_CMD="./bass"
    PY3_CMD="python3"
else
    BASS_CMD="./bass.exe"
    PY3_CMD="py -3"
fi

if [ "$CLEAN" = "-clean" ]; then
    MAKE_CMD="make clean && make"
else
    MAKE_CMD="make -j"
fi

if [ "$CONVERT_FILES" = "-c" ]; then
    $PY3_CMD tool/hConv.py scene include
    $PY3_CMD tool/hConv.py object include
    $PY3_CMD tool/hConv.py images include
    $PY3_CMD tool/hConv.py other include
    $PY3_CMD tool/hConv.py audio include
    $PY3_CMD tool/hConv.py text include
fi

printf "Compiling...\n"

cd src/ddTool
eval "$MAKE_CMD" ||  { echo "Build failed"; exit 1; }
cd ../filesystem
eval "$MAKE_CMD" || { echo "Build failed"; exit 1; }
cd ../diskCode
eval "$MAKE_CMD" ||  { echo "Build failed"; exit 1; }
cd ../diskBoot
eval "$MAKE_CMD" ||  { echo "Build failed"; exit 1; }
cd ../diskSystem
eval "$MAKE_CMD" ||  { echo "Build failed"; exit 1; }

cd ..
cd ..

printf "OK.\n\n"

rm -f *.ndr
rm -f *.disk
rm -f *.ndd

$BASS_CMD ./asm/EZLJ_DISK_Main.asm -d "$VERSION_PARAM"
