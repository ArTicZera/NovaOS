/*
    Coded by ArTic/JhoPro
    
    Here we have a lot of drawing functions and frames manipulation.
    Take a look at it!
*/

#include "../Include/stdint.h"
#include "../Memory/alloc.h"
#include "../Timer/timer.h"

#include "graphics.h"

DWORD vidmem;
DWORD pitch;

void InitGraphics(DWORD fb, DWORD p)
{
    vidmem = fb;
    pitch = p;
}

DWORD GetFramebuffer()
{
    return vidmem;
}

DWORD GetPitch()
{
    return pitch;
}

void SetPixel(int x, int y, DWORD color)
{
    LPDWORD framebuffer = (LPDWORD) vidmem;

    framebuffer[x + (y * pitch / 4)] = color;
}

//Here its the opposite. We read from the buffer index
DWORD GetPixel(int x, int y)
{
    LPDWORD framebuffer = (LPDWORD) vidmem;

    return framebuffer[x + (y * pitch / 4)];
}

//Just fills it with black (0x00)
void ClearScreen(void)
{
    for (int y = 0; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, 0x00);
        }
    }
}

//Draw a rectangle in some area
void DrawRect(int x, int y, int w, int h, DWORD color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            SetPixel(j, i, color);
        }
    }
}

void DrawCircle(int cx, int cy, int radius, DWORD color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (y >= x)
    {
        // 8 pontos simétricos
        SetPixel(cx + x, cy + y, color);
        SetPixel(cx - x, cy + y, color);
        SetPixel(cx + x, cy - y, color);
        SetPixel(cx - x, cy - y, color);

        SetPixel(cx + y, cy + x, color);
        SetPixel(cx - y, cy + x, color);
        SetPixel(cx + y, cy - x, color);
        SetPixel(cx - y, cy - x, color);

        x++;

        if (d < 0)
        {
            d += 2 * x + 1;
        }
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void Blit(LPDWORD src, int dstX, int dstY, int w, int h)
{
    int startX = 0, startY = 0;
    int endX = w, endY = h;

    if (dstX < 0) startX = -dstX;
    if (dstY < 0) startY = -dstY;
    if (dstX + w > WSCREEN) endX = WSCREEN - dstX;
    if (dstY + h > HSCREEN) endY = HSCREEN - dstY;

    for (int y = startY; y < endY; y++)
    {
        int screenY = dstY + y;

        LPDWORD srcRow = &src[y * w + startX];
        LPDWORD dstRow = &((LPDWORD)vidmem)[screenY * WSCREEN + (dstX + startX)];

        int length = endX - startX;

        for (int i = 0; i < length; i++)
            dstRow[i] = srcRow[i];
    }
}