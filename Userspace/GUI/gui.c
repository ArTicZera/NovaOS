#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
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
//extern DWORD terminal[];
extern DWORD non[];
extern DWORD txt[32 * 32];
extern DWORD dir[32 * 32];
//extern DWORD exe[32 * 32];
//extern BYTE sprite[128 * 64];

void DrawStartMenu(WINDOW window)
{
    DrawRect(window.x, window.y, window.w, window.h, window.color);

    SetCursorX(window.x / 2 + 50);
    SetCursorY(window.y + 22);

    Print("Admin", 0xFF00FFFF);
}

/*
void DrawDesktopIcon(EXTICON exticon, char* name, int posX, int posY)
{
    int iconSize = 64;

    int srcX = index * iconSize;
    int srcY = 0;

    for (int y = 0; y < iconSize; y++)
    {
        for (int x = 0; x < iconSize; x++)
        {
            DWORD color = sprite[(y) * (128) + (x)];

            if (!(color == 0x00000000)) 
            {
                SetPixel(x, y, color);
            }
        }
    }
}*/

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

/*
void DrawDesktopIcon(EXTICON exticon, char* name, int posX, int posY)
{
    LPDWORD iconBitmap = NULL;

    switch (exticon)
    {
        case ICON_DIR:
            iconBitmap = dir;
            break;
        default:
            return;
    }

    for (int y = 0; y < 32; y++)
    {
        int srcY = 64 - y;

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
*/