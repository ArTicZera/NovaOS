#!/usr/bin/env python3

import sys

if len(sys.argv) < 3:
    print("Usage:")
    print("python bdf2hex.py fonte.bdf font.c")
    exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

glyphs = {}

encoding = None
bitmap = []
width = 0
height = 0

inside_bitmap = False

with open(input_file, "r", encoding="utf-8", errors="ignore") as f:
    for line in f:
        line = line.strip()

        if line.startswith("FONTBOUNDINGBOX"):
            parts = line.split()
            width = int(parts[1])
            height = int(parts[2])

        elif line.startswith("ENCODING"):
            encoding = int(line.split()[1])

        elif line == "BITMAP":
            bitmap = []
            inside_bitmap = True

        elif line == "ENDCHAR":
            inside_bitmap = False
            if encoding is not None:
                glyphs[encoding] = bitmap

        elif inside_bitmap:
            bitmap.append(int(line, 16))

bytes_per_row = (width + 7) // 8

with open(output_file, "w") as out:

    out.write("// Gerado automaticamente\n\n")
    out.write("#include <stdint.h>\n\n")

    out.write(f"#define WFONT {width}\n")
    out.write(f"#define HFONT {height}\n\n")

    out.write("const uint8_t font[] = {\n")

    for ch in range(256):

        bmp = glyphs.get(ch)

        if bmp is None:
            bmp = [0] * height

        out.write(f"    // {ch}\n")

        for row in bmp:

            for b in range(bytes_per_row):

                shift = (bytes_per_row - b - 1) * 8
                value = (row >> shift) & 0xFF
                out.write(f"0x{value:02X}, ")

            out.write("\n")

        out.write("\n")

    out.write("};\n")