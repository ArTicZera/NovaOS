#ifndef GRAPHICS_H
#define GRAPHICS_H

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
DWORD GetPixel(int x, int y);
void ClearScreen(void);
void DrawRect(int x, int y, int w, int h, DWORD color);
void DrawCircle(int cx, int cy, int radius, DWORD color);
void DrawLine(int x0, int y0, int x1, int y1, DWORD color);
void Blit(LPDWORD src, int dstX, int dstY, int w, int h);
void BlendPixel(int x, int y, DWORD color, BYTE alpha);

#endif
