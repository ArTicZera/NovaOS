#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Include/math.h"
#include "../Interrupts/idt.h"
#include "../Timer/timer.h"
#include "../Font/text.h"

#include "../Wayland/protocol.h"
#include "../Wayland/surface.h"
#include "../Wayland/compositor.h"

#include "clock.h"

static Clock clock;
static WLWindow* clk = NULL;

void InitClock()
{
    int x = 1080, y = 440, w = 200, h = 270;

    clk = WLCreateWindow(x, y, w, h, "Clock");
}

void ClockIRQ()
{
    if (!clk)
    {
        return;
    }
          
    int cx = clk->w / 2;
    int cy = clk->h / 2;

    RoundRect(clk->buffer, clk->w, clk->h, 0, 0, clk->w, clk->h, 5, 0xFF1A1A1A);

    DrawCircle(clk, clk->w, clk->h, cx, cy, 95, 0xFFFFFFFF);
    DrawHourMarks(clk, cx, cy, 95, 0xFFFFFFFF);

    ClockUpdate(&clock);
    DrawClock(&clock, clk, cx, cy);

    CommitSurface(clk);
}

void ClockUpdate(Clock *clk)
{
    clk->hours = hour;
    clk->minutes = minute;
    clk->seconds = second;

    clk->angleS = (clk->seconds / 60.0) * (2.0 * PI) - PI / 2.0;

    clk->angleM = (clk->minutes / 60.0) * (2.0 * PI) - PI / 2.0;

    clk->angleH = ((clk->hours % 12) * 60 + clk->minutes) / 720.0 * (2.0 * PI) - PI / 2.0;
}

void ClockPointer(double angle, int size, int *x, int *y)
{
    *x = (int)(cos(angle) * size);
    *y = (int)(sin(angle) * size);
}

void DrawPointer(WLWindow* win, int cx, int cy, double angle, int size, DWORD color)
{
    int x,y;

    ClockPointer(angle, size, &x, &y);

    DrawLine(cx, cy, cx + x, cy + y, win->w, win->h, win->buffer, color);
}

void DrawClock(Clock *clk, WLWindow* win, int cx, int cy)
{
    DrawPointer(win, cx, cy, clk->angleH, 40, 0xFFFFFFFF);
    DrawPointer(win, cx, cy, clk->angleM, 60, 0xFFFFFFFF);
    DrawPointer(win, cx, cy, clk->angleS, 80, 0xFFFF0000);
}

void  DrawHourMarks(WLWindow* win, int cx, int cy, int radius, DWORD color)
{
    for (int i = 0; i < 12; i++)
    {
        double angle = (2.0 * PI * i) / 12.0 - PI / 2.0;

        int x1 = cx + (int)(cos(angle) * (radius - 10));
        int y1 = cy + (int)(sin(angle) * (radius - 10));

        int x2 = cx + (int)(cos(angle) * radius);
        int y2 = cy + (int)(sin(angle) * radius);

        DrawLine(x1, y1, x2, y2, win->w, win->h, win->buffer, color);
    }
}