/*
    This will return the events of the desktop, like opening an explorer, 
    opening a program or even create files, etc.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../FileSystem/memfs.h"
#include "../Shell/shell.h"
#include "../ELF/elf.h"
#include "../Drivers/keyboard.h"

#include "GUI/win.h"
#include "GUI/gui.h"
#include "userspace.h"
#include "desktop.h"

extern char doom[];

DesktopItem items[MAXITEMS];
int totItems = 0;

DesktopItem* GetClickedItem(int x, int y)
{
    for (int i = 0; i < totItems; i++)
    {
        if (x >= items[i].x && x < items[i].x + 100 && y >= items[i].y && y < items[i].y + 85)
        {
            return &items[i];
        }
    }

    return 0x00;
}

void HandleDesktopClick(int x, int y, int pressed)
{
    static int lastPressed = 0;
    static DesktopItem* lastItem = NULL;
    static DWORD lastClickTime = 0;

    if (pressed && !lastPressed)
    {
        DesktopItem* item = GetClickedItem(x, y);

        if (item)
        {
            if (item == lastItem)
            {
                if (strcmp(item->name, "Terminal") == 0)
                {
                    WINDOW* win = CreateWindow(80, 80, 650, 432, 0xFF1A1A1A, "Terminal");
                    StartShellGUI(win);
                }
                else if (strcmp(item->name, "DOOM") == 0)
                {
                    CreateWindow(320, 160, 640, 400, 0xFF1A1A1A, "DOOM");
                    KeyboardState(6);
                    LoadELF(doom, 0);
                }

                lastItem = NULL;
                lastClickTime = 0;
            }
            else
            {
                lastItem = item;
                lastClickTime = 0;
            }
        }
        else
        {
            lastItem = NULL;
        }
    }

    lastPressed = pressed;
}

void SetDesktopIndex(char* name, int type, int x, int y)
{
    if (totItems < MAXITEMS)
    {
        strncpy(items[totItems].name, name, 15);

        items[totItems].name[15] = '\0';
        items[totItems].type = type;
        items[totItems].x = x;
        items[totItems].y = y;

        totItems++;
    }
}
