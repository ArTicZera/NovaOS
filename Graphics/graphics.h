#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../Include/stdint.h"
#include "../Wayland/protocol.h"

#define WSCREEN 1280
#define HSCREEN 720

typedef struct
{
    LPDWORD buffer;
    int w, h;
} FRAMEBUFFER;

void InitGraphics(DWORD fb, DWORD p);
DWORD GetFramebuffer();
DWORD GetPitch();
void SetPixel(int x, int y, DWORD color);
void BufferSetPixel(LPDWORD buffer, int w, int h, int x, int y, DWORD color);
DWORD GetPixel(int x, int y);
void ClearScreen(void);
void DrawRect(int x, int y, int w, int h, DWORD color);
void DrawCircle(WLWindow* win, int w, int h, int cx, int cy, int radius, DWORD color);
void DrawLine(int x0, int y0, int x1, int y1, int w, int h, DWORD* buffer, DWORD color);
void Blit(LPDWORD src, int dstX, int dstY, int w, int h);

void CopyScreenToBuffer(LPDWORD dst, int srcX, int srcY, int w, int h);
void BlendPixel(int x, int y, DWORD color, BYTE alpha);

void BitBlt(int srcX, int srcY, int dstX, int dstY, int w, int h);
void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void AlphaBlend(int x, int y, int w, int h, DWORD color, BYTE alpha);

#endif
