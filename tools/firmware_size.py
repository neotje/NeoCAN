from elftools.elf.elffile import ELFFile
from elftools.elf.descriptions import describe_sh_flags
import os
import argparse

MICROCONTROLLERS = {
    "RPi-Pico": {
        "flash_size": 2_097_152 , # 2MB
        "ram_size": 264_192, # 264KB
    }
}

# Function to find all .elf files in a directory
def find_elf_files(directory):
    elf_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".elf"):
                elf_files.append(os.path.join(root, file))
    return elf_files

# Collect sections info from an ELF file
def collect_sections_info(elf_file: ELFFile):
    sections = {}

    for section in elf_file.iter_sections():
        if section.is_null() or section.name.startswith('.debug'):
            continue

        section_type = section['sh_type']
        section_size = section.data_size
        section_flags = describe_sh_flags(section['sh_flags'])

        sections[section.name] = {
            'type': section_type,
            'size': section_size,
            'flags': section_flags
        }

    return sections

def is_flash_section(section):
    return section.get("type", "") == "SHT_PROGBITS" and "A" in section.get("flags", "")

def is_ram_section(section):
    return (
        section.get("type", "") in ("SHT_NOBITS", "SHT_PROGBITS")
        and section.get("flags", "") == "WA"
    )

# Caclulate firmware size
def calculate_firmware_size(sections):
    flash_size = ram_size = 0

    for section_info in sections.values():
        if is_flash_section(section_info):
            flash_size += section_info.get("size", 0)
        
        if is_ram_section(section_info):
            ram_size += section_info.get("size", 0)

    return flash_size, ram_size

if __name__ == "__main__":
    # Extract folder and mcu from command line arguments
    boards = [board for board in MICROCONTROLLERS.keys()]

    parser = argparse.ArgumentParser(
        description="Calculate firmware size from .elf files"
    )
    parser.add_argument("folder", help="Folder containing .elf files")
    parser.add_argument("-m", "--mcu-board", help="MCU name", default=boards[0], choices=boards)
    args = parser.parse_args()

    # find all .elf files in the folder
    elf_files = find_elf_files(args.folder)
    mcu = MICROCONTROLLERS[args.mcu_board]

    for elf_file_path in elf_files:
        with open(elf_file_path, 'rb') as f:
            elf_file = ELFFile(f)
            sections = collect_sections_info(elf_file)
            flash_size, ram_size = calculate_firmware_size(sections)

            print(f"File: {os.path.basename(elf_file_path)}")
            print(f"Flash: {flash_size} / {mcu['flash_size']} ({flash_size / mcu['flash_size'] * 100:.2f}%)")
            print(f"RAM: {ram_size} / {mcu['ram_size']} ({ram_size / mcu['ram_size'] * 100:.2f}%)")
            print()