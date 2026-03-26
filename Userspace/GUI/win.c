#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Font/text.h"
#include "../../Memory/alloc.h"
#include "../userspace.h"

#include "win.h"

int StartWindowXY = 80;

WINDOW* windowsList  = 0;
WINDOW* activeWindow = 0;

WINDOW* CreateWindow(int w, int h, DWORD color, char* title)
{
    WINDOW* win = (WINDOW*)AllocateMemory(sizeof(WINDOW));

    if (!win) return NULL;

    win->x = StartWindowXY;
    win->y = StartWindowXY;

    win->w = w;
    win->h = h;
    win->color = color;
    win->title = title;

    win->next = NULL;
    win->prev = NULL;

    AddWindow(win);

    StartWindowXY += 40;

    if (StartWindowXY > 200)
    {
        StartWindowXY = 80;
    }

    activeWindow = win;

    DrawAllWindows();

    return win;
}

void DrawWindow(WINDOW window, int nextWindow)
{
    DrawRoundedRect(window.x, window.y, window.w, window.h, 5, window.color);

    if (window.title)
    {
        SetCursorX(window.x + 10);
        SetCursorY(window.y +  4);

        Print(window.title, 0xFFFFFFFF);
    }

    if (nextWindow)
    {
        StartWindowXY += 40;

        if (StartWindowXY > 200)
        {
            StartWindowXY = 80;
        }
    }
}

void DrawAllWindows()
{
    WINDOW* current = windowsList;

    while (current && current->next)
    {
        current = current->next;
    }

    while (current)
    {
        DrawWindow(*current, 0);
        current = current->prev;
    }
}

void AddWindow(WINDOW* window)
{
    window->next = windowsList;
    window->prev = 0;

    if (windowsList)
    {
        windowsList->prev = window;
    }
    windowsList = window;
}

void BringToFront(WINDOW* win)
{
    if (win == windowsList) return;

    if (win->prev)
        win->prev->next = win->next;

    if (win->next)
        win->next->prev = win->prev;

    win->next = windowsList;
    win->prev = NULL;

    if (windowsList)
        windowsList->prev = win;

    windowsList = win;
}

void DrawRoundedRect(int x, int y, int w, int h, int radius, DWORD color)
{
    int right = x + w;
    int bottom = y + h;

    DWORD pixelColor = color;

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x + radius; j < right - radius; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
    }

    for (int i = x + radius; i < right - radius; i++) 
    {
        for (int j = y; j < y + radius; j++) 
        {
            SetPixel(i, j, pixelColor);
        }

        for (int j = bottom - radius; j < bottom; j++) 
        {
            SetPixel(i, j, pixelColor);
        }
    }

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x; j < x + radius; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
        for (int j = right - radius; j < right; j++) 
        {
            SetPixel(j, i, pixelColor);
        }
    }

    for (int dx = -radius; dx <= radius; dx++) 
    {
        for (int dy = -radius; dy <= radius; dy++) 
        {
            if (dx * dx + dy * dy <= radius * radius) 
            {
                SetPixel(x + radius + dx, y + radius + dy, pixelColor);

                SetPixel(right - radius - 1 + dx, y + radius + dy, pixelColor);

                SetPixel(x + radius + dx, bottom - radius - 1 + dy, pixelColor);

                SetPixel(right - radius - 1 + dx, bottom - radius - 1 + dy, pixelColor);
            }
        }
    }
}

void HandleWindowDragging(int mouseX, int mouseY, int pressed)
{
    static int isDragging = 0;
    static int offsetX = 0;
    static int offsetY = 0;
    static WINDOW* dragWindow = NULL;
    static int lastPressed = 0;

    if (pressed && !lastPressed)
    {
        WINDOW* win = GetWindowAt(mouseX, mouseY);

        if (win)
        {
            if (mouseY >= win->y && mouseY <= win->y + 20)
            {
                dragWindow = win;
                isDragging = 1;

                offsetX = mouseX - win->x;
                offsetY = mouseY - win->y;

                BringToFront(win);
                activeWindow = win;
            }
        }
    }

    if (pressed && isDragging && dragWindow)
    {
        dragWindow->x = mouseX - offsetX;
        dragWindow->y = mouseY - offsetY;

        if (dragWindow->x < 0) dragWindow->x = 0;
        if (dragWindow->y < 0) dragWindow->y = 0;
        if (dragWindow->x + dragWindow->w > WSCREEN)
            dragWindow->x = WSCREEN - dragWindow->w;
        if (dragWindow->y + dragWindow->h > HSCREEN)
            dragWindow->y = HSCREEN - dragWindow->h;

        DrawAllWindows();
    }

    if (!pressed)
    {
        isDragging = 0;
        dragWindow = NULL;
    }

    lastPressed = pressed;
}

WINDOW* GetWindowAt(int x, int y)
{
    WINDOW* current = windowsList;

    while (current)
    {
        if (x >= current->x && x <= current->x + current->w && y >= current->y && y <= current->y + current->h)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void HandleMouseClick(int x, int y, int pressed)
{
    static int lastPressed = 0;

    if (pressed && !lastPressed)
    {
        WINDOW* win = GetWindowAt(x, y);

        if (win)
        {
            BringToFront(win);
            activeWindow = win;
        }
        else
        {
            activeWindow = NULL;

            if (x > WSCREEN - 100 && y > HSCREEN - 100)
            {
                CreateWindow(200, 150, 0xFF1A1A1A, "Nova Window");
            }
        }

        DrawAllWindows();
    }

    lastPressed = pressed;
}
