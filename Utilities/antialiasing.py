from PIL import Image, ImageFilter
import re

INPUT = "stdfont.h"
OUTPUT = "font_antialiased.h"

UPSCALE = 2

with open(INPUT, "r", encoding="utf-8") as f:
    text = f.read()

m = re.search(
    r'BYTE\s+isoFont\s*\[\]\s*=\s*\{(.*?)\};',
    text,
    re.DOTALL
)

if not m:
    raise Exception("isoFont[] não encontrado.")

array = m.group(1)

data = [int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{2})', array)]

if len(data)%16 != 0:
    raise Exception(f"Fonte inválida ({len(data)} bytes).")

glyphs = len(data)//16

print("Caracteres:",glyphs)

out = []

for g in range(glyphs):

    glyph = data[g*16:(g+1)*16]

    img = Image.new("L",(8,16),0)

    pixels = img.load()

    for y in range(16):
        b = glyph[y]

        for x in range(8):

            if b & (1<<(7-x)):
                pixels[x,y]=255

    img = img.resize((8*UPSCALE,16*UPSCALE),Image.NEAREST)

    img = img.filter(ImageFilter.GaussianBlur(radius=UPSCALE/3))

    img = img.resize((8,16),Image.LANCZOS)

    p = img.load()

    out.append("{")

    for y in range(16):

        line=[]

        for x in range(8):

            v=p[x,y]

            if v<5:
                v=0

            line.append(str(v))

        out.append("    {"+", ".join(line)+"},")

    out.append("},")

with open(OUTPUT,"w") as f:

    f.write("unsigned char isoFontAA[%d][16][8] =\n{\n"%glyphs)

    for line in out:
        f.write(line+"\n")

    f.write("};\n")

print("Gerado:",OUTPUT)