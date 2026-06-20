#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "win.h"
#include "gui.h"

void DrawClock(int cx, int cy, int radius, int hours, int minutes, int seconds)
{
    DrawCircle(cx, cy, radius, 0xFFFFFFFF);
}

void HandleDrawClock()
{
    
}