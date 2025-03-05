#include "../Include/stdint.h"

#include "graphics.h"
#include "novagl.h"

void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    float sx = (float)w1 / w2;
    float sy = (float)h1 / h2;

    for (int dy = 0; dy < h2; dy++)
    {
        for (int dx = 0; dx < w2; dx++)
        {
            int sx_pos = (int)(x1 + dx * sx);
            int sy_pos = (int)(y1 + dy * sy);

            DWORD color = framebuffer[sy_pos * WSCREEN + sx_pos];

            framebuffer[(y2 + dy) * WSCREEN + (x2 + dx)] = color;
        }
    }
}
