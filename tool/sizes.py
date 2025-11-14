#!/usr/bin/env python3
import os
import struct
import argparse

# Set up command-line argument parsing
parser = argparse.ArgumentParser(description="Process a binary file with padding and address adjustments.")
parser.add_argument("file", help="The binary file to process")

# Parse the arguments
args = parser.parse_args()
FILE = args.file

# Check if the file exists
if not os.path.isfile(FILE):
    print(f"Error: The file {FILE} does not exist.")
    exit(1)

# Get the size of the file
size = os.path.getsize(FILE)

pad_to = 32
padding = (pad_to - (size % pad_to)) % pad_to

if padding != 0:
    with open(FILE, "ab") as f:
        f.write(b"\x00" * padding)

new_size = size + padding
addr_plus = new_size + 0x80400000
addr_plus |= 0xA0000000

with open(FILE, "r+b") as f:
    f.seek(0x1064)
    f.write(struct.pack(">I", new_size))      # big-endian size

    f.seek(0x106C)
    f.write(struct.pack(">I", addr_plus))     # big-endian size + base

print(f"File '{FILE}' processed successfully.")
