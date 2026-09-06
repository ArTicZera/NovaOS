/*
    Coded by ArTic/JhoPro
    
    Here we have a lot of drawing functions and frames manipulation.
    Take a look at it!
*/

#include "../Include/stdint.h"
#include "../Include/math.h"
#include "../Memory/alloc.h"
#include "../Timer/timer.h"

#include "../Wayland/protocol.h"

#include "graphics.h"

extern LPDWORD backBuffer;

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

void BufferSetPixel(LPDWORD buffer, int w, int h, int x, int y, DWORD color)
{
    if(x < 0 || y < 0 || x >= w || y >= h) return;

    buffer[y * w + x] = color;
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

void DrawCircle(WLWindow* win, int w, int h, int cx, int cy, int radius, DWORD color)
{
    for(double a = 0; a < 2 * PI; a += 0.01)
    {
        int x = cx + cos(a) * radius;
        int y = cy + sin(a) * radius;

        BufferSetPixel(win->buffer, w, h, x, y, color);
    }
}

void DrawLine(int x0, int y0, int x1, int y1, int w, int h, DWORD* buffer, DWORD color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        BufferSetPixel(buffer, w, h, x0, y0, color);
        //SetPixel(x0,y0,color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
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

void BitBlt(int srcX, int srcY, int dstX, int dstY, int w, int h)
{
    LPDWORD framebuffer = (LPDWORD)GetFramebuffer();

    LPDWORD temp = (LPDWORD)AllocateMemory(w * h * sizeof(DWORD));

    if (!temp) return;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            temp[y * w + x] = framebuffer[(srcY + y) * WSCREEN + (srcX + x)];
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            framebuffer[(dstY + y) * WSCREEN + (dstX + x)] = temp[y * w + x];
        }
    }

    FreeMemory(temp);
}

void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    LPDWORD framebuffer = (LPDWORD) GetFramebuffer();

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

void CopyScreenToBuffer(LPDWORD dst, int srcX, int srcY, int w, int h)
{
    int startX = 0, startY = 0;
    int endX = w, endY = h;

    if (srcX < 0) startX = -srcX;
    if (srcY < 0) startY = -srcY;
    if (srcX + w > WSCREEN) endX = WSCREEN - srcX;
    if (srcY + h > HSCREEN) endY = HSCREEN - srcY;

    LPDWORD framebuffer = (LPDWORD)GetFramebuffer();

    for (int y = startY; y < endY; y++)
    {
        int screenY = srcY + y;

        LPDWORD srcRow = &framebuffer[screenY * WSCREEN + (srcX + startX)];
        LPDWORD dstRow = &dst[y * w + startX];

        int length = endX - startX;

        for (int i = 0; i < length; i++)
        {
            dstRow[i] = srcRow[i];
        }
    }
}

void BlendPixel(int x, int y, DWORD color, BYTE alpha)
{
    DWORD dst = GetPixel(x, y); // você precisa ter uma função que leia o framebuffer

    BYTE sr = (color >> 16) & 0xFF;
    BYTE sg = (color >> 8)  & 0xFF;
    BYTE sb =  color        & 0xFF;

    BYTE dr = (dst >> 16) & 0xFF;
    BYTE dg = (dst >> 8)  & 0xFF;
    BYTE db =  dst        & 0xFF;

    BYTE r = (sr * alpha + dr * (255 - alpha)) / 255;
    BYTE g = (sg * alpha + dg * (255 - alpha)) / 255;
    BYTE b = (sb * alpha + db * (255 - alpha)) / 255;

    SetPixel(x, y, (0xFF << 24) | (r << 16) | (g << 8) | b);
}

void AlphaBlend(int x, int y, int w, int h, DWORD color, BYTE alpha)
{
    if (x < 0 || y < 0 || x >= w || y >= h) return;

    DWORD *framebuffer = (DWORD *) GetFramebuffer();

    DWORD dst = framebuffer[y * w + x];

    DWORD sr = (color >> 16) & 0xFF;
    DWORD sg = (color >> 8)  & 0xFF;
    DWORD sb =  color        & 0xFF;

    DWORD dr = (dst >> 16) & 0xFF;
    DWORD dg = (dst >> 8)  & 0xFF;
    DWORD db =  dst        & 0xFF;

    DWORD r = (sr * alpha + dr * (255 - alpha)) / 255;
    DWORD g = (sg * alpha + dg * (255 - alpha)) / 255;
    DWORD b = (sb * alpha + db * (255 - alpha)) / 255;

    framebuffer[y * w + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
}
