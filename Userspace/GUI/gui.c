/*
    Coded by ArTic/JhoPro

    GUI functions and etc.
*/

#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Font/text.h"
#include "../../Timer/timer.h"

#include "gui.h"
#include "../userspace.h"

int StartWindowXY = 80;

//Import icons
extern BYTE error[];
extern BYTE warning[];
extern BYTE info[];
extern BYTE question[];
extern BYTE user[];
extern BYTE user2[];
extern BYTE terminal[];
extern BYTE non[];
extern BYTE txt[];
extern BYTE dir[];
extern BYTE exe[];

//Draw a simple window using DrawRoundedRect and with the buttons
void DrawWindow(WINDOW window, int nextWindow)
{
    DrawRoundedRect(window.x, window.y, window.w, window.h, 5, 0x13);

    if (window.title)
    {
        SetCursorX(window.x + 10);
        SetCursorY(window.y +  4);

        Print(window.title, 0x0F);
    }

    DrawWindowButtons(window);

    if (nextWindow)
    {
        StartWindowXY += 40;

        if (StartWindowXY > 200)
        {
            StartWindowXY = 80;
        }
    }
}

void DrawStartMenu(WINDOW window)
{
    DrawRect(window.x, window.y, window.w, window.h, window.color);

    DrawIcon(window.x + 15, window.y + 15, ICON_USER1);

    SetCursorX(window.x + 67);
    SetCursorY(window.y + 22);

    Print("User", 0x34);
}

void DrawIcon(int startX, int startY, ICON icon)
{
    const BYTE* iconBitmap = NULL;

    switch (icon)
    {
        case ICON_ERROR:
            iconBitmap = error;
            break;
        case ICON_WARNING:
            iconBitmap = warning;
            break;
        case ICON_INFO:
            iconBitmap = info;
            break;
        case ICON_QUESTION:
            iconBitmap = question;
            break;
        case ICON_USER1:
            iconBitmap = user;
            break;
        case ICON_TERMINAL:
            iconBitmap = terminal;
            break;
        default:
            return;
    }

    for (int y = 0; y < 16; y++) 
    {
        for (int x = 0; x < 16; x++) 
        {
            BYTE color = iconBitmap[y * 16 + (x + 8)];

            if (color != 0x00 && color != 28)
            {
                SetPixel(startX + x * 2, startY + y * 2, color);
                SetPixel(startX + x * 2 + 1, startY + y * 2, color);
                SetPixel(startX + x * 2, startY + y * 2 + 1, color);
                SetPixel(startX + x * 2 + 1, startY + y * 2 + 1, color);
            }
        }
    }
}

void DrawDesktopIcon(EXTICON exticon, char* name, int x, int y)
{
    const BYTE* iconBitmap = NULL;

    switch (exticon)
    {
        case ICON_TEXT:
            iconBitmap = txt;
            break;
        case ICON_DIR:
            iconBitmap = dir;
            break;
        case ICON_EXE:
            iconBitmap = exe;
            break;
        case ICON_DEFAULT:
            iconBitmap = non;
            break;
        case ICON_USER2:
            iconBitmap = user2;
            break;
        default:
            return;
    }

    for (int ys = 0; ys < 32; ys++)
    {
        for (int xs = x; xs < 32; xs++)
        {
            BYTE color = iconBitmap[ys * 32 + xs];

            if (color != 0x00 && color != 28)
            {
                SetPixel(x + xs * 2, y + ys * 2, color);
                SetPixel(x + xs * 2 + 1, y + ys * 2, color);
                SetPixel(x + xs * 2, y + ys * 2 + 1, color);
                SetPixel(x + xs * 2 + 1, y + ys * 2 + 1, color);
            }
        }
    }

    SetCursorX(x + 20);
    SetCursorY(y + 60);

    Print(name, 0x0F);
}

void DrawMessageBox(WINDOW msgbox, ICON icon, char* text, int nextWindow)
{
    DrawWindow(msgbox, nextWindow);

    DrawIcon(msgbox.x + 25, msgbox.y + 35, icon);

    int saveX = GetCursorX();
    int saveY = GetCursorY();

    SetCursorX(msgbox.x + 68);
    SetCursorY(msgbox.y + 40);

    Print(text, 0x0F);

    SetCursorX(saveX);
    SetCursorY(saveY);

    GetWindow(msgbox);
}

void DrawRoundedRect(int x, int y, int w, int h, int radius, BYTE color)
{
    int right = x + w;
    int bottom = y + h;

    BYTE pixelColor = color;

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x + radius; j < right - radius; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
    }

    for (int i = x + radius; i < right - radius; i++) 
    {
        for (int j = y; j < y + radius; j++) 
        {
            SetPixel(i, j, pixelColor);
        }

        for (int j = bottom - radius; j < bottom; j++) 
        {
            SetPixel(i, j, pixelColor);
        }
    }

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x; j < x + radius; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
        for (int j = right - radius; j < right; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
    }

    for (int dx = -radius; dx <= radius; dx++) 
    {
        for (int dy = -radius; dy <= radius; dy++) 
        {
            if (dx * dx + dy * dy <= radius * radius) 
            {
                SetPixel(x + radius + dx, y + radius + dy, pixelColor);

                SetPixel(right - radius - 1 + dx, y + radius + dy, pixelColor);

                SetPixel(x + radius + dx, bottom - radius - 1 + dy, pixelColor);

                SetPixel(right - radius - 1 + dx, bottom - radius - 1 + dy, pixelColor);
            }
        }
    }
}

void DrawButton(int x, int y, int size, const char* icon, BYTE color)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (icon[i * size + j] == '1')
            {
                SetPixel(x + j, y + i, color);
                SetPixel(x + j + 1, y + i, color);
            }
        }
    }
}

void DrawWindowButtons(WINDOW window)
{
    const int buttonSize = 10;

    const char closeIcon[] =
    {
        '1','0','0','0','1',
        '0','1','0','1','0',
        '0','0','1','0','0',
        '0','1','0','1','0',
        '1','0','0','0','1',
    };

    DrawButton(window.x + window.w - buttonSize - 10, window.y + 5, 5, closeIcon, 0x0F);

    const char maximizeIcon[] =
    {
        '1','1','1','1','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','1','1','1','1',
    };

    DrawButton(window.x + window.w - (2 * buttonSize) - 20, window.y + 5, 5, maximizeIcon, 0x0F);

    const char minimizeIcon[] =
    {
        '0','0','0','0','0',
        '0','0','0','0','0',
        '1','1','1','1','1',
        '0','0','0','0','0',
        '0','0','0','0','0',
    };

    DrawButton(window.x + window.w - (3 * buttonSize) - 30, window.y + 5, 5, minimizeIcon, 0x0F);
}

WINDOW ReturnWindow(WINDOW window)
{
    return window;
}

int GetStartWindowXY()
{
    return StartWindowXY;
}

void WindowClear(WINDOW window)
{
    DrawWindow(window, 0x00);
}
