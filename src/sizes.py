#!/usr/bin/env python3
import os
import struct

FILE = "diskC.bin"

size = os.path.getsize(FILE)

pad_to = 32
padding = (pad_to - (size % pad_to)) % pad_to

if padding != 0:
    with open(FILE, "ab") as f:
        f.write(b"\x00" * padding)

new_size = size + padding
addr_plus = new_size + 0x80400000

with open(FILE, "r+b") as f:
    f.seek(0x1064)
    f.write(struct.pack(">I", new_size))      # big-endian size

    f.seek(0x106C)
    f.write(struct.pack(">I", addr_plus))     # big-endian size + base
