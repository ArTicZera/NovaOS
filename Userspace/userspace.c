#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Graphics/novagl.h"
#include "../Hardware/cmos.h"
#include "../Drivers/keyboard.h"
#include "../Drivers/mouse.h"
#include "../Memory/mem.h"
#include "../Memory/alloc.h"
#include "../Font/text.h"
#include "../FileSystem/memfs.h"
#include "../Timer/timer.h"
#include "../Drivers/sb16.h"

#include "GUI/win.h"
#include "GUI/gui.h"
#include "login.h"
#include "desktop.h"
#include "userspace.h"

extern BYTE bootscr[];
extern BYTE backgrd[];

extern BYTE bootAudio[];

WINDOW startMenu = { "", 0xFF1A1A1A, 0, HSCREEN - 285, 150, 250, 0 };
WINDOW* winmgr;

int inUserSpace = 0x00;

int isStartMenuOpen = 0;
int isMaximizedOpen = 0;   
int wasMousePressed = 0;

int currentPID = 0;
int totalWindows = 0;

DWORD startBuffer[150 * 200];

DWORD* backbuffer;

void DrawBackground()
{
    int bytes_per_pixel = 3;
    int bytes_per_row = WSCREEN * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            int img_y = (HSCREEN - 1 - y);
            int index = (img_y * bytes_per_row) + (x * bytes_per_pixel) + (img_y * padding);

            BYTE b = backgrd[index];
            BYTE g = backgrd[index + 1];
            BYTE r = backgrd[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            SetPixel(x - 18, y, color);
        }
    }
}

void DrawTaskbar()
{
    DWORD* blurbuffer = AllocateMemory(WSCREEN * HSCREEN * 4);

    // 1️⃣ aplica blur NO BACKBUFFER
    BlurRegion(backbuffer, blurbuffer, 0, HSCREEN - 35, WSCREEN, 35, 1);

    for (int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        memcpy(&backbuffer[y * WSCREEN], &blurbuffer[y * WSCREEN], WSCREEN * 4);
    }
}

void SaveStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            startBuffer[y * GetPitch() / 4 + x] = GetPixel(x, y);
        }
    }
}

void RestoreStartArea()
{
    for (int y = HSCREEN - (startMenu.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < startMenu.w; x++)
        {
            SetPixel(x, y, startBuffer[y * GetPitch() / 4 + x]);
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

void UpdateExplorer()
{
    DrawBackground();
    DesktopIcons();
    DrawTaskbar();

    for (int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, backbuffer[y * WSCREEN + x]);
        }
    }

    SetCursorX(30);
    SetCursorY(HSCREEN - 25);
    Print("START", 0xFFFFFFFF);

    SetCursorX(WSCREEN - 65);
    SetCursorY(HSCREEN - 25);
    GetCMOSDate();
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

            int x = 64;
            int y = 20 + (index * 76);
            DrawDesktopIcon(icon, filename, x, y);
            SetDesktopIndex(filename, 0x01, x, y);

            index++;
        }
    }
}



void DrawBootScr(void)
{
    int bytes_per_pixel = 3;
    int bytes_per_row = 128 * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            int img_y = (129 - 1 - y);
            int index = (img_y * bytes_per_row) + ((x - 110) * bytes_per_pixel) + (img_y * padding);
            
            int dx = WSCREEN / 2 - 64;
            int dy = HSCREEN / 2 - 128;

            BYTE b = bootscr[index];
            BYTE g = bootscr[index + 1];
            BYTE r = bootscr[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            SetPixel(x + dx, y + dy, color);
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

    int lastPressed = 0;

    while (TRUE)
    {
        int x, y, pressed;
        GetMouseState(&x, &y, &pressed);

        StartButtonHandler(x, y, pressed);

        HandleMouseClick(x, y, pressed);

        HandleWindowDragging(x, y, pressed);
    }
}
