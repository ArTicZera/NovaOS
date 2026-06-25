#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Font/text.h"
#include "../../Timer/timer.h"

#include "win.h"
#include "gui.h"
#include "../userspace.h"

#include "../../Include/Icons/DIR.h"

extern DWORD error[];
extern DWORD warning[];
extern DWORD info[];
extern DWORD question[];
extern DWORD user[];
extern DWORD user2[];
//extern DWORD terminal[];
extern DWORD non[];
extern DWORD txt[32 * 32];
//extern DWORD exe[32 * 32];
//extern BYTE sprite[128 * 64];

void DrawStartMenu(WINDOW window)
{
    DrawRect(window.x, window.y, window.w, window.h, window.color);

    SetCursorX(window.x / 2 + 50);
    SetCursorY(window.y + 22);

    Print("Admin", 0xFF00FFFF);
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

    /*
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
    }*/

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            if (iconBitmap[y * 32 + x] == 0xFF000000)
            {
                continue;
            }

            SetPixel(posX + x + 16, posY + y, iconBitmap[y * 32 + x]);
        }
    }

    SetCursorX(posX + 20);
    SetCursorY(posY + 36);
    Print(name, 0xFFFFFFFF);
}
