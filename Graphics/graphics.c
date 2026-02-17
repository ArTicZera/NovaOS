/*
    Coded by ArTic/JhoPro
    
    Here we have a lot of drawing functions and frames manipulation.
    Take a look at it!
*/

#include "../Include/stdint.h"
#include "../Timer/timer.h"

#include "graphics.h"

DWORD vidmem;
DWORD pitch;

void InitGraphics(LPDWORD mbinfo)
{
    vidmem = ((QWORD)mbinfo[23] << 32) | mbinfo[22];
    pitch = mbinfo[24];
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
