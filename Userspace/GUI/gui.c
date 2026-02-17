#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Font/text.h"
#include "../../Timer/timer.h"

#include "gui.h"
#include "../userspace.h"

int StartWindowXY = 80;

extern DWORD error[];
extern DWORD warning[];
extern DWORD info[];
extern DWORD question[];
extern DWORD user[];
extern DWORD user2[];
extern DWORD terminal[];
extern DWORD non[];
extern DWORD txt[];
extern BYTE dir[];
extern DWORD exe[];

void DrawWindow(WINDOW window, int nextWindow)
{
    DrawRoundedRect(window.x, window.y, window.w, window.h, 5, window.color);

    if (window.title)
    {
        SetCursorX(window.x + 10);
        SetCursorY(window.y +  4);

        Print(window.title, 0xFFFFFFFF);
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

    SetCursorX(window.x / 2 + 50);
    SetCursorY(window.y + 22);

    Print("Admin", 0xFF00FFFF);
}

void DrawDesktopIcon(EXTICON exticon, char* name, int x, int y)
{
    const DWORD* iconBitmap = NULL;

    switch (exticon)
    {
        case ICON_TEXT:
            iconBitmap = txt;
            break;
        case ICON_DIR:
            iconBitmap = txt;
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

    int bytes_per_pixel = 3;
    int bytes_per_row = 64 * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            int img_y = (64 - 1 - y);
            int index = (img_y * bytes_per_row) + (x * bytes_per_pixel) + (img_y * padding);
            
            BYTE b = dir[index];
            BYTE g = dir[index + 1];
            BYTE r = dir[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            SetPixel(x, y, color);
        }
    }

    /*
    int bytes_per_pixel = 3;
    int bytes_per_row = 32 * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            int img_y = (HSCREEN - 1 - y);
            int index = (img_y * bytes_per_row) + (x * bytes_per_pixel) + (img_y * padding);

            BYTE b = dir[index];
            BYTE g = dir[index + 1];
            BYTE r = dir[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            SetPixel(x - 18, y, color);
        }
    }*/

    SetCursorX(x + 20);
    SetCursorY(y + 60);

    Print(name, 0xFFFFFFFF);
}

void DrawMessageBox(WINDOW msgbox, ICON icon, char* text, int nextWindow)
{
    DrawWindow(msgbox, nextWindow);

    //DrawIcon(msgbox.x + 25, msgbox.y + 35, icon);

    int saveX = GetCursorX();
    int saveY = GetCursorY();

    SetCursorX(msgbox.x + 68);
    SetCursorY(msgbox.y + 40);

    Print(text, 0xFFFFFFFF);

    SetCursorX(saveX);
    SetCursorY(saveY);

    GetWindow(msgbox);
}

void DrawRoundedRect(int x, int y, int w, int h, int radius, DWORD color)
{
    int right = x + w;
    int bottom = y + h;

    DWORD pixelColor = color;

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

void DrawButton(int x, int y, int size, const char* icon, DWORD color)
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

    DrawButton(window.x + window.w - buttonSize - 10, window.y + 5, 5, closeIcon, 0xFFFFFFFF);

    const char maximizeIcon[] =
    {
        '1','1','1','1','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','1','1','1','1',
    };

    DrawButton(window.x + window.w - (2 * buttonSize) - 20, window.y + 5, 5, maximizeIcon, 0xFFFFFFFF);

    const char minimizeIcon[] =
    {
        '0','0','0','0','0',
        '0','0','0','0','0',
        '1','1','1','1','1',
        '0','0','0','0','0',
        '0','0','0','0','0',
    };

    DrawButton(window.x + window.w - (3 * buttonSize) - 30, window.y + 5, 5, minimizeIcon, 0xFFFFFFFF);
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
