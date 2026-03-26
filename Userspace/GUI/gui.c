#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Include/bmp.h"
#include "../../Font/text.h"
#include "../../Timer/timer.h"

#include "win.h"
#include "gui.h"
#include "../userspace.h"

extern DWORD error[];
extern DWORD warning[];
extern DWORD info[];
extern DWORD question[];
extern DWORD user[];
extern DWORD user2[];
extern DWORD terminal[];
extern DWORD non[];
extern DWORD txt[32 * 32];
extern DWORD dir[32 * 32];
extern DWORD exe[32 * 32];

void DrawStartMenu(WINDOW window)
{
    DrawRect(window.x, window.y, window.w, window.h, window.color);

    SetCursorX(window.x / 2 + 50);
    SetCursorY(window.y + 22);

    Print("Admin", 0xFF00FFFF);
}

void BlurRegion(DWORD* src, DWORD* dst, int x0, int y0, int w, int h, int radius)
{
    for (int y = y0; y < y0 + h; y++)
    {
        for (int x = x0; x < x0 + w; x++)
        {
            int r = 0, g = 0, b = 0, count = 0;

            for (int ky = -radius; ky <= radius; ky++)
            {
                for (int kx = -radius; kx <= radius; kx++)
                {
                    int px = x + kx;
                    int py = y + ky;

                    if (px < 0 || py < 0 || px >= WSCREEN || py >= HSCREEN)
                        continue;

                    DWORD c = src[py * WSCREEN + px];

                    r += (c >> 16) & 0xFF;
                    g += (c >> 8) & 0xFF;
                    b += c & 0xFF;
                    count++;
                }
            }

            r /= count;
            g /= count;
            b /= count;

            dst[y * WSCREEN + x] =
                0xFF000000 |
                (r << 16) |
                (g << 8) |
                b;
        }
    }
}

void DrawDesktopIcon(EXTICON exticon, char* name, int posX, int posY)
{
    const DWORD* iconBitmap = NULL;

    switch (exticon)
    {
        //case ICON_TEXT:    iconBitmap = txt;  break;
        case ICON_DIR:     iconBitmap = dir;  break;
        //case ICON_EXE:     iconBitmap = exe;  break;
        //case ICON_DEFAULT: iconBitmap = non;  break;
        //case ICON_USER2:   iconBitmap = user2; break;
        default: return;
    }

    for (int y = 0; y < 32; y++)
    {
        int srcY = 31 - y;

        for (int x = 0; x < 32; x++)
        {
            if (iconBitmap[srcY * 32 + x] == 0x00)
            {
                continue;
            }

            DWORD color = iconBitmap[srcY * 32 + x];
            SetPixel(posX + x + 16, posY + y, color);
        }
    }

    SetCursorX(posX + 20);
    SetCursorY(posY + 36);
    Print(name, 0xFFFFFFFF);
}
