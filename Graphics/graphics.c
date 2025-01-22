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

//Here it becomes interesting. This was based on GDI by Microsoft
void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    LPBYTE framebuffer = (LPBYTE) VIDMEM;

    //Calculate for scaling 
    float sx = (float)w1 / w2;
    float sy = (float)h1 / h2;

    for (int dy = 0; dy < h2; dy++)
    {
        for (int dx = 0; dx < w2; dx++)
        {
            //Get the index
            int sx_pos = (int)(x1 + dx * sx);
            int sy_pos = (int)(y1 + dy * sy);

            //Gets the pixel from a index
            DWORD color = framebuffer[sy_pos * WSCREEN + sx_pos];

            //Set it to another
            framebuffer[(y2 + dy) * WSCREEN + (x2 + dx)] = color;
        }
    }
}
