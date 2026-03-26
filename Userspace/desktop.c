/*
    This will return the events of the desktop, like opening an explorer, 
    opening a program or even create files, etc.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../FileSystem/memfs.h"

#include "GUI/win.h"
#include "GUI/gui.h"
#include "userspace.h"
#include "desktop.h"

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
