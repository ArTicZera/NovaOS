#ifndef GUI_H
#define GUI_H

typedef enum
{
    ICON_NONE,
    ICON_ERROR,
    ICON_WARNING,
    ICON_INFO,
    ICON_QUESTION,
    ICON_USER1,
} ICON;

typedef enum
{
    ICON_TEXT,
    ICON_DIR,
    ICON_EXE,
    ICON_DOOM,
    ICON_TERMINAL,
    ICON_DEFAULT,
    ICON_USER2,
} EXTICON;


void DrawStartMenu(WINDOW window);
void DrawDesktopIcon(EXTICON exticon, char* name, int posX, int posY);
//void DrawIcon(int index, int dstX, int dstY);

#endif