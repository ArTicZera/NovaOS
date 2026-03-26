typedef struct WINDOW
{
    const char* title;
    DWORD color;

    int x, y;
    int w, h;

    int dragOffsetX;
    int dragOffsetY;

    struct WINDOW* next;
    struct WINDOW* prev;
} WINDOW;

void DrawWindow(WINDOW window, int nextWindow);
void AddWindow(WINDOW* window);
void BringToFront(WINDOW* win);
void HandleMouseClick(int x, int y, int pressed);
void DrawAllWindows();
WINDOW* GetWindowAt(int x, int y);
WINDOW* CreateWindow(int w, int h, DWORD color, char* title);
void DrawRoundedRect(int x, int y, int w, int h, int radius, DWORD color);
void HandleWindowDragging(int mouseX, int mouseY, int mousePressed);
