#!/bin/sh

VERSION_PARAM="USA"
MAKE_FS="nofs"
CLEAN="noclean"

while [ "$#" -gt 0 ]; do
    case "$1" in
        -fs|--fs|fs)
            MAKE_FS="-fs"
            ;;
        -clean|--clean|-c|--c|c|clean)
            CLEAN="-c"
            ;;
        -*)
            echo "Unknown option: $1"
            exit 1
            ;;
        *)
            VERSION_PARAM="$1"
            ;;
    esac
    shift
done

OS_TYPE=$(uname -s 2>/dev/null || echo Windows)

if [ "$OS_TYPE" = "Linux" ]; then
    BASS_CMD="./bass"
    PY3_CMD="python3"
else
    BASS_CMD="./bass.exe"
    PY3_CMD="py -3"
fi

if [ "$CLEAN" = "-c" ]; then
    MAKE_CMD="make clean && make"
else
    MAKE_CMD="make"
fi

if [ "$MAKE_FS" = "-fs" ]; then
    $PY3_CMD tool/hConv.py scene include
    $PY3_CMD tool/hConv.py object include
    $PY3_CMD tool/hConv.py images include
    $PY3_CMD tool/hConv.py other include
    $PY3_CMD tool/hConv.py audio include
    $PY3_CMD tool/hConv.py text include
fi

printf "Compiling...\n"
cd src
eval "$MAKE_CMD" "$VERSION_PARAM" || { echo "Build failed"; exit 1; }
cd ..
printf "OK.\n\n"