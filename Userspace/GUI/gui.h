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

typedef struct
{
    const BYTE* title;
    BYTE color;

    int x, y;
    int w, h;

    int isMaximized;
} WINDOW;

typedef struct GUIEvent
{
    int type;
    int mouseX, mouseY;
    int keyCode;
    int windowID; 
} GUIEvent;

typedef struct GraphicsBuffer
{
    int width, heights;
    BYTE* data;
} GraphicsBuffer;

void DrawWindow(WINDOW window, int nextWindow);
void DrawWindowTransition(WINDOW window);
void DrawRoundedRect(int x, int y, int w, int h, int radius, BYTE color);
void DrawButton(int x, int y, int size, const char* icon, BYTE color);
void DrawMessageBox(WINDOW msgbox, ICON icon, char* text, int nextWindow);
void DrawIcon(int startX, int startY, ICON icon);
void DrawWindowButtons(WINDOW window);
void WindowClear(WINDOW window);
void DrawStartMenu(WINDOW window);
void DrawDesktopIcon(EXTICON exticon, char* name, int x, int y);

int GetStartWindowXY();
WINDOW ReturnWindow(WINDOW window);
