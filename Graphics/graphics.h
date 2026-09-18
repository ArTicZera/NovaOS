#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../Include/stdint.h"
#include "../Wayland/protocol.h"

#define WSCREEN 1280
#define HSCREEN 720

struct HDC
{
    int unused;
}; typedef struct HDC* LPHDC;

#define SRCCOPY      0x00CC0020
#define SRCPAINT     0x00EE0086
#define SRCAND       0x008800C6
#define SRCINVERT    0x00660046
#define SRCERASE     0x00440328
#define NOTSRCCOPY   0x00330008
#define NOTSRCERASE  0x001100A6
#define NOTSRCINVERT 0x99999999
#define MERGECOPY    0x00C000CA
#define MERGEPAINT   0x00BB0226
#define PATCOPY      0x00F00021
#define PATPAINT     0x00FB0A09
#define PATINVERT    0x005A0049
#define DSTINVERT    0x00550009
#define BLACKNESS    0x00000042
#define WHITENESS    0x00FF0062

typedef struct
{
    LPDWORD buffer;
    int w, h;
} FRAMEBUFFER;

typedef union RGBQUAD
{
    DWORD rgb;
    struct
    {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE reserved;
    };
} RGBQUAD_t;

typedef RGBQUAD_t* PRGBQUAD;

typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT v;
} HSV;

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

void BitBlt(int srcX, int srcY, int dstX, int dstY, int w, int h, DWORD rop);
void StretchBlt(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, DWORD rop);
void AlphaBlend(int x, int y, int w, int h, DWORD color, BYTE alpha);

static float HueToRGB(float p, float q, float t);

RGBQUAD_t HSLtoRGB(HSL hsl);
HSL RGBtoHSL(RGBQUAD_t rgb);

RGBQUAD_t HSVtoRGB(HSV hsv);
HSV RGBtoHSV(RGBQUAD_t rgb);

void HueFunction();

#endif
