#ifndef WIN_H
#define WIN_H

#include "../../Include/stdint.h"

#define MAX_WINDOWS 10

typedef struct WINDOW
{
    const char* title;
    DWORD color;

    int x, y;
    int w, h;

    int dragOffsetX;
    int dragOffsetY;

    int visible;
    int dragging;
    int focused;

    LPDWORD buffer;

    struct WINDOW* next;
    struct WINDOW* prev;
} WINDOW;

extern int StartWindowXY;

void DrawWindow(WINDOW window, int nextWindow);
void AddWindow(WINDOW* window);
void BringToFront(WINDOW* win);
void HandleMouseClick(int x, int y, int pressed);
void DrawAllWindows();
WINDOW* GetWindowAt(int x, int y);
WINDOW* CreateWindow(int x, int y,int w, int h, DWORD color, char* title);
void DrawRoundedRect(int x, int y, int w, int h, int radius, DWORD color);
void HandleWindowDragging(int mouseX, int mouseY, int mousePressed);

void DrawButton(int x, int y, int size, const char* icon, DWORD color);
void DrawWindowButtons(WINDOW window);
void CloseWindow(int x, int y, int pressed);
void ForceCloseWindow(WINDOW* win);

void SaveBackgroundArea(WINDOW* win);
void SaveWindowArea(WINDOW* win);
void RestoreWindowArea(WINDOW* win);

#endif