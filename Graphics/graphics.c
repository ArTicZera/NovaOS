/*
    Coded by ArTic/JhoPro
    
    Here we have a lot of drawing functions and frames manipulation.
    Take a look at it!
*/

#include "../Include/stdint.h"
#include "../Timer/timer.h"

#include "graphics.h"

//Loads the video memory and create a buffer.
//Then we just draws a pixel in [y * WSCREEN + x] index.
void SetPixel(int x, int y, BYTE color)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    framebuffer[y * WSCREEN + x] = color;
}

//Here its the opposite. We read from the buffer index
BYTE GetPixel(int x, int y)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    return framebuffer[y * WSCREEN + x];
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
void DrawRect(int x, int y, int w, int h, BYTE color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            SetPixel(j, i, color);
        }
    }
}
