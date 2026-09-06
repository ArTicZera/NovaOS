#ifndef WAYLAND_SURFACE_H
#define WAYLAND_SURFACE_H

#include "../Include/stdint.h"

#define WL_MAX_SURFACES 64
#define MAX_DIRTY_RECTS 128

typedef struct
{
    int used;

    DWORD id;

    int client;

    int x;
    int y;

    int oldX;
    int oldY;

    int width;
    int height;

    char title[64];

    LPDWORD buffer;
    LPDWORD background;

    int dirty;

} WLSurface;

typedef struct 
{
    int x;
    int y;
    int w;
    int h;
} DirtyRect;


typedef struct
{
    DWORD surface;
    DWORD buffer;

    DWORD w;
    DWORD h;
} WLAttachBufferRequest;

void WLSurfaceInit();

void WLCreateSurfaceFunc(DWORD id, int x, int y, int w, int h);

int PointInsideSurface(WLSurface *surface, int x, int y);

void WLDestroySurface(DWORD id);

WLSurface *WLFindSurface(DWORD id);

#endif