import re

SRC_W = 16
SRC_H = 32

DST_W = 8
DST_H = 16

SCALE_X = SRC_W // DST_W
SCALE_Y = SRC_H // DST_H

bytes_per_row = SRC_W // 8
glyph_size = bytes_per_row * SRC_H

with open("spleen16x32.h") as f:
    text = f.read()

data = [int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{2})', text)]

glyphs = len(data) // glyph_size

output = []

for g in range(glyphs):

    glyph = data[g*glyph_size:(g+1)*glyph_size]

    for y in range(DST_H):

        byte = 0

        for x in range(DST_W):

            count = 0

            for sy in range(SCALE_Y):
                for sx in range(SCALE_X):

                    xx = x*SCALE_X + sx
                    yy = y*SCALE_Y + sy

                    b = glyph[yy*bytes_per_row + xx//8]
                    bit = (b >> (7-(xx%8))) & 1

                    count += bit

            if count >= (SCALE_X*SCALE_Y)//2:
                byte |= 1 << (7-x)

        output.append(byte)

with open("spleen8x16.c","w") as f:

    f.write("#define WFONT 8\n")
    f.write("#define HFONT 16\n\n")
    f.write("unsigned char font[]={\n")

    for i,b in enumerate(output):

        f.write(f"0x{b:02X},")

        if (i+1)%16==0:
            f.write("\n")

    f.write("};\n")