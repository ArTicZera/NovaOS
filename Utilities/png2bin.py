from PIL import Image

INPUT = "logo.png"
OUTPUT = "logo.h"
ARRAY_NAME = "bootscr"

img = Image.open(INPUT).convert("RGBA")
width, height = img.size

with open(OUTPUT, "w") as f:
    f.write("// Gerado automaticamente\n\n")
    f.write(f"#define WFONT {width}\n")
    f.write(f"#define HFONT {height}\n\n")

    f.write(f"DWORD {ARRAY_NAME}[] =\n{{\n")

    count = 0

    for y in range(height):
        f.write("    ")

        for x in range(width):
            r, g, b, a = img.getpixel((x, y))

            color = (a << 24) | (r << 16) | (g << 8) | b

            f.write(f"0x{color:08X}, ")

            count += 1
            if count % 8 == 0:
                f.write("\n    ")

        f.write("\n")

    f.write("};\n")

print("Concluído!")