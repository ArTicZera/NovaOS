/*
    This will return the events of the desktop, like opening an explorer, 
    opening a program or even create files, etc.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../FileSystem/memfs.h"

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

void DesktopEvents(int x, int y, int pressed)
{
    if (pressed)
    {
        DesktopItem* item = GetClickedItem(x, y);
        
        if (item)
        {
            if (item->type == 0)
            {
                char title[32];
                const char* baseTitle = "Explorer - ";
                int i = 0;

                while (*baseTitle != '\0') 
                {
                    title[i++] = *baseTitle++;
                }

                const char* dirName = item->name;

                while (*dirName != '\0') 
                {
                    title[i++] = *dirName++;
                }

                title[i] = '\0';

                int pos = GetStartWindowXY();
                WINDOW window = { title, 0x12, pos, pos, 320, 200 };
                DrawWindow(window, 0x00);
                GetWindow(window);

                FileSystem* fs = (FileSystem*) FSADDRESS;
                Directory* selectedDir = NULL;

                for (int i = 0; i < MAXSUBDIR; i++)
                {
                    if (fs->root.subdirs[i] != NULL && strcmp(fs->root.subdirs[i]->name, item->name) == 0)
                    {
                        selectedDir = fs->root.subdirs[i];
                        
                        break;
                    }
                }

                if (selectedDir != NULL)
                {
                    int fileX = pos + 10;
                    int fileY = pos + 40;

                    for (int i = 0; i < MAXFILES; i++)
                    {
                        if (selectedDir->files[i].filename[0] != '\0')
                        {
                            DrawDesktopIcon(ICON_DEFAULT, selectedDir->files[i].filename, fileX, fileY);
                            
                            fileX += 100;
                        }
                    }
                }
            }
            else if (item->type == 1)
            {
                Print(item->name, 0x0B);
            }
        }
    }
}
