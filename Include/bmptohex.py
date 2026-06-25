from PIL import Image

INPUT_FILE = "dir.bmp"
OUTPUT_FILE = "image.h"
ARRAY_NAME = "test"

img = Image.open(INPUT_FILE).convert("RGB")
width, height = img.size
pixels = img.load()

with open(OUTPUT_FILE, "w") as f:
    f.write(f"// Generated from {INPUT_FILE}\n")
    f.write(f"#define {ARRAY_NAME.upper()}_WIDTH {width}\n")
    f.write(f"#define {ARRAY_NAME.upper()}_HEIGHT {height}\n\n")

    f.write(f"unsigned int {ARRAY_NAME}[] =\n{{\n")

    count = 0

    for y in range(height):
        f.write("    ")
        for x in range(width):
            r, g, b = pixels[x, y]
            color = (0xFF << 24) | (r << 16) | (g << 8) | b
            f.write(f"0x{color:08X}")

            count += 1
            if count != width * height:
                f.write(", ")

        f.write("\n")

    f.write("};\n")