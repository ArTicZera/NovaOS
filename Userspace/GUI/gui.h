typedef enum
{
    ICON_NONE,
    ICON_ERROR,
    ICON_WARNING,
    ICON_INFO,
    ICON_QUESTION,
    ICON_USER1,
    ICON_TERMINAL
} ICON;

typedef enum
{
    ICON_TEXT,
    ICON_DIR,
    ICON_EXE,
    ICON_DEFAULT,
    ICON_USER2
} EXTICON;

void DrawStartMenu(WINDOW window);
void DrawDesktopIcon(EXTICON exticon, char* name, int x, int y);
void BlurRegion(DWORD* src, DWORD* dst, int x0, int y0, int w, int h, int radius);
