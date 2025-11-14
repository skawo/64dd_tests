import os
import re
import argparse

# Allowed extensions
ALLOWED_EXTENSIONS = {".zmap", ".zscene", ".bin"}

def sanitize_name(filename):
    """Convert filename to a valid C identifier in uppercase."""
    name, ext = os.path.splitext(filename)
    full_name = f"{name}_{ext[1:]}" if ext else name
    full_name = re.sub(r'\W', '_', full_name)
    return full_name.upper()

def process_file(input_path, output_folder, array_name):
    """Convert and save each file as a C header."""
    guard_name = f"{array_name}_H"
    output_path = os.path.join(output_folder, f"{array_name}.h")

    with open(input_path, "rb") as f:
        content = f.read()

    with open(output_path, "w") as f:
        f.write(f"#ifndef {guard_name}\n")
        f.write(f"#define {guard_name}\n\n")
        f.write(f"unsigned char {array_name}[] = {{\n")

        for i, byte in enumerate(content):
            f.write(f"0x{byte:02X}, ")
            if (i + 1) % 12 == 0:
                f.write("\n")

        f.write(f"\n}};\n")
        f.write(f"unsigned int {array_name}_LEN = {len(content)};\n\n")
        f.write(f"#endif // {guard_name}\n")

    print(f"Generated {output_path}")

def main(input_folder, output_folder):
    # Create the base output folder structure: src/fileHeaders/[first input folder name]
    base_output_folder = os.path.join(output_folder, 'fileHeaders', input_folder)
    os.makedirs(base_output_folder, exist_ok=True)

    # Walk through the directory recursively
    for root, dirs, files in os.walk(input_folder):
        for filename in files:
            _, ext = os.path.splitext(filename)
            if ext.lower() not in ALLOWED_EXTENSIONS:
                continue  # Skip files with disallowed extensions

            input_path = os.path.join(root, filename)

            # Create a relative path from the input folder, and sanitize the file name
            relative_path = os.path.relpath(root, input_folder)
            sanitized_name = sanitize_name(filename)

            # Calculate the subfolder path in the output based on the directory structure
            output_subfolder = os.path.join(base_output_folder, relative_path)
            os.makedirs(output_subfolder, exist_ok=True)

            # Process the file
            process_file(input_path, output_subfolder, sanitized_name)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert files to C headers")
    parser.add_argument("input_folder", help="Folder containing input files")
    parser.add_argument("output_folder", help="Folder to save generated headers")
    args = parser.parse_args()

    main(args.input_folder, args.output_folder)
