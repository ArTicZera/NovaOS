#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Hardware/cmos.h"
#include "../Drivers/keyboard.h"
#include "../Drivers/mouse.h"
#include "../Memory/mem.h"
#include "../Font/text.h"
#include "../FileSystem/memfs.h"
#include "../Timer/timer.h"

#include "GUI/gui.h"
#include "login.h"
#include "desktop.h"
#include "userspace.h"

extern BYTE bootscr[];
extern BYTE backgrd[];

WINDOW startMenu = { "", 0x12, 0, HSCREEN - 285, 150, 250, 0 };
WINDOW* winmgr;

int inUserSpace = 0x00;

int isStartMenuOpen = 0;
int isMaximizedOpen = 0;   
int wasMousePressed = 0;

int currentWindowID = 0;
int totalWindows = 0;

BYTE startBuffer[150 * 200];

void DrawBackground()
{
    for (int y = 0; y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, backgrd[y * WSCREEN + (x + 8)]);
        }
    }
}

void DrawTaskbar()
{
    for (int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, 0x13);
        }
    }

    SetCursorX(30);
    SetCursorY(HSCREEN - 25);

    Print("START", 0x0F);

    SetCursorX(WSCREEN - 65);
    SetCursorY(HSCREEN - 25);

    GetCMOSDate();
}

void SaveStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            startBuffer[y * 150 + x] = GetPixel(x, y);
        }
    }
}

void RestoreStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            SetPixel(x, y, startBuffer[y * 150 + x]);
        }
    }
}

void StartButtonHandler(int x, int y, int pressed)
{
    int startX = 0;
    int endX = 100;

    int startY = HSCREEN - 35;
    int endY = HSCREEN;

    if (pressed && !wasMousePressed && x >= startX && x < endX && y >= startY && y < endY)
    {
        if (isStartMenuOpen)
        {
            RestoreStartArea();
            isStartMenuOpen = 0;
        }
        else
        {
            SaveStartArea();
            DrawStartMenu(startMenu);
            isStartMenuOpen = 1;
        }
    }
    
    wasMousePressed = pressed;
}

void GetWindow(WINDOW window)
{
    winmgr[currentWindowID] = window;

    currentWindowID++;
    totalWindows++;
}

void MaximizeWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - (2 * buttonSize) - 20;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            if (win->isMaximized % 2 == 0)
            {
                //SaveTotalArea();

                win->x = 0;
                win->y = 0;
                win->w = WSCREEN;
                win->h = HSCREEN - 32;

                win->isMaximized++;

                DrawWindow(*win, 0x00);
            }
            else
            {
                win->x = GetStartWindowXY();
                win->y = GetStartWindowXY();
                win->w = 320;
                win->h = 200;

                win->isMaximized++;

                DrawBackground();

                DrawWindow(*win, 0x00);
            }

            if (strcmp(win->title, "NovaOS - Run") == 0x00)
            {
                //StartShell(*win);
            }
        }
    }
}

void CloseWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - buttonSize - 10;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            for (int j = i; j < totalWindows - 1; j++)
            {
                winmgr[j] = winmgr[j + 1];
            }

            totalWindows--;

            UpdateExplorer();

            break;
        }
    }
}

void MinimizeWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - (3 * buttonSize) - 30;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            UpdateExplorer();
        }
    }
}

void UpdateExplorer()
{
    DrawBackground();
    DrawTaskbar();
    DesktopIcons();
}

void UserspaceState(int state)
{
    if (state == FALSE)
    {
        inUserSpace = 0x00;
    }
    else
    {
        inUserSpace = 0x01;
    }
}

void DesktopIcons()
{
    FileSystem* fs = (FileSystem*) FSADDRESS;
    
    int index = 0;

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (fs->root.subdirs[i] != NULL && fs->root.subdirs[i]->name[0] != '\0')
        {
            int x = 10;
            int y = 10 + (index * 76);
            DrawDesktopIcon(ICON_DIR, fs->root.subdirs[i]->name, x, y);
            SetDesktopIndex(fs->root.subdirs[i]->name, 0x00, x, y);

            index++;
        }
    }

    for (int i = 0; i < MAXFILES; i++)
    {
        if (fs->root.files[i].filename[0] != '\0')
        {
            char* filename = fs->root.files[i].filename;
            const char* extension = NULL;

            for (const char* p = filename; *p != '\0'; p++) 
            {
                if (*p == '.') 
                {
                    extension = p + 1;
                }
            }

            EXTICON icon;

            if (extension != NULL) 
            {
                if (strcmp(extension, "txt") == 0) 
                {
                    icon = ICON_TEXT;
                } 
                else if (strcmp(extension, "exe") == 0) 
                {
                    icon = ICON_EXE;
                } 
                else 
                {
                    icon = ICON_DEFAULT;
                }
            } 
            else 
            {
                icon = ICON_DEFAULT;
            }

            int x = 10;
            int y = 10 + (index * 76);
            DrawDesktopIcon(icon, filename, x, y);
            SetDesktopIndex(filename, 0x01, x, y);

            index++;
        }
    }
}

void HandleWindowDragging(WINDOW* window, int mouseX, int mouseY, int mousePressed)
{
    static int isDragging = 0;
    static int offsetX = 0;
    static int offsetY = 0;
    
    if (!window) return;

    if (mousePressed) 
    {
        if (!isDragging) 
        {
            if (mouseX >= window->x && mouseX <= window->x + window->w - 100 && mouseY >= window->y && mouseY <= window->y + 20) 
            {
                isDragging = 1;
                offsetX = mouseX - window->x;
                offsetY = mouseY - window->y;
            }
        }

        if (isDragging) 
        {
            window->x = mouseX - offsetX;
            window->y = mouseY - offsetY;

            if (window->x < 0) window->x = 0;
            if (window->y < 0) window->y = 0;
            if (window->x + window->w > WSCREEN) window->x = WSCREEN - window->w;
            if (window->y + window->h > HSCREEN) window->y = HSCREEN - window->h;

            UpdateExplorer();
            DrawWindow(*window, 0);
        }
    } 
    else 
    {
        isDragging = 0;
    }
}

void DrawBootScr(void)
{
    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            //Calculate Center
            int fx = WSCREEN / 2 - 64;
            int fy = HSCREEN / 2 - 96;

            //Draw Bitmap
            SetPixel(x + fx, y + fy, bootscr[y * 128 + (x + 8)]);
        }
    }

    DrawLoadBar();
    ClearScreen();
}

void UserSpace()
{
    while (!inUserSpace)
    {

    }

    KeyboardState(3);

    LoginScreen();

    KeyboardState(1);

    UpdateExplorer();

    while (true)
    {
        int x, y, pressed;
        GetMouseState(&x, &y, &pressed);

        DesktopEvents(x, y, pressed);

        StartButtonHandler(x, y, pressed);

        MaximizeWindow(x, y, pressed);
        MinimizeWindow(x, y, pressed);
        CloseWindow(x, y, pressed);

        HandleWindowDragging(&winmgr[currentWindowID - 1], x, y, pressed);
    }
}