#include "../Include/stdint.h"
#include "../Memory/mem.h"

#include "surface.h"

WLSurface surfaces[WL_MAX_SURFACES];

void WLSurfaceInit()
{
    memset(surfaces, 0, sizeof(surfaces));
}

WLSurface* WLFindSurface(DWORD id)
{
    for(int i = 0; i < WL_MAX_SURFACES; i++)
    {
        if(surfaces[i].used && surfaces[i].id == id) 
        {
            return &surfaces[i];
        }
    }

    return NULL;
}

/*
WLSurface* WLCreateSurfaceFunc(DWORD id, int x, int y, int w, int h)
{
    for(int i = 0; i < WL_MAX_SURFACES; i++)
    {
        if(!surfaces[i].used)
        {
            surfaces[i].used = 1;

            surfaces[i].id = id;

            surfaces[i].x = x;
            surfaces[i].y = y;

            surfaces[i].width = w;
            surfaces[i].height = h;

            return &surfaces[i];
        }
    }
    return NULL;
}
*/

void WLCreateSurfaceFunc(DWORD id, int x, int y, int w, int h)
{
    for(int i = 0; i < WL_MAX_SURFACES; i++)
    {
        if(!surfaces[i].used)
        {
            surfaces[i].used = 1;

            surfaces[i].id = id;

            surfaces[i].x = x;
            surfaces[i].y = y;

            surfaces[i].oldX = x;
            surfaces[i].oldY = y;

            surfaces[i].width = w;
            surfaces[i].height = h;

            //surfaces[i].background =
            //    AllocateMemory(
            //        w * h * sizeof(DWORD)
            //    );

            return;
        }
    }
}

int PointInsideSurface(WLSurface *surface, int x, int y)
{
    if(surface == NULL)
        return 0;


    if(x >= surface->x && x < surface->x + surface->width &&
       y >= surface->y && y < surface->y + surface->height)
    {
        return 1;
    }


    return 0;
}

void WLDestroySurface(DWORD id)
{
    WLSurface* s = WLFindSurface(id);

    if(s)
    {
        memset(s, 0, sizeof(WLSurface));
    }
}