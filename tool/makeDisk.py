import sys
import os

def build_ndd(target):
    # Define file mappings and offsets
    target_map = {
        "USA":      "EZLE.ndd",
        "USA-DEV":  "EZLE-DEV.ndd",
        "JPN":      "EZLJ.ndd",
        "JPN-DEV":  "EZLJ-DEV.ndd",
        "USA-D64":  "EZLE.D64",
        "JPN-D64":  "EZLJ.D64"
    }

    if target not in target_map:
        raise ValueError(f"Unknown target: {target}")

    output_file = target_map[target]
    is_D64 = target.endswith("D64")

    script_dir = os.path.dirname(os.path.abspath(__file__))
    disk_system_path = os.path.join(script_dir, "../src/diskSystem/diskSystem.bin")
    disk_boot_path   = os.path.join(script_dir, "../src/diskBoot/diskBoot.bin")
    disk_code_path   = os.path.join(script_dir, "../src/diskCode/diskCode.bin")

    with open(disk_system_path, "rb") as f:
        disk_system = f.read()
    with open(disk_boot_path, "rb") as f:
        disk_boot = f.read()
    with open(disk_code_path, "rb") as f:
        disk_code = f.read()

    if is_D64:
        output = bytearray()
        output_size = max(0x200 + 0x4F08 + len(disk_code), 0x200 + len(disk_boot), len(disk_system))
        output.extend(b'\x00' * output_size)
        output[0x0:0x0+len(disk_system)] = disk_system
        output[0x200:0x200+len(disk_boot)] = disk_boot
        output[0x200+0x4D08:0x200+0x4D08+len(disk_code)] = disk_code        
    else:
        output_size = 0x3DEC800
        output = bytearray(b'\x00' * output_size)
        output[0x0:0x0+len(disk_system)] = disk_system
        output[0x738C0:0x738C0+len(disk_boot)] = disk_boot
        output[0x738C0+0x4D08:0x738C0+0x4D08+len(disk_code)] = disk_code

    with open(output_file, "wb") as f:
        f.write(output)

    print(f"Created {output_file} successfully.")
    
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python build_ndd.py <TARGET>")
        print("Targets: USA, USA-DEV, JPN, JPN-DEV, USA-D64, JPN-D64")
        sys.exit(1)
    
    target_arg = sys.argv[1].strip()
    build_ndd(target_arg)

