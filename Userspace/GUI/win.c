#include "../../Include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Font/text.h"
#include "../../Memory/alloc.h"
#include "../../Memory/mem.h"
#include "../../Timer/timer.h"
#include "../userspace.h"

#include "win.h"

int StartWindowXY = 80;

WINDOW* windowsList  = 0;
WINDOW* activeWindow = 0;

LPDWORD winArea;

WINDOW* CreateWindow(int x, int y, int w, int h, DWORD color, char* title)
{
    WINDOW* win = (WINDOW*)AllocateMemory(sizeof(WINDOW));

    if (!win) return NULL;

    win->x = x;
    win->y = y;

    win->w = w;
    win->h = h;

    win->color = color;
    win->title = title;

    win->next = NULL;
    win->prev = NULL;

    win->buffer = AllocateMemory(win->w * win->h * 4);

    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            win->buffer[y * win->w + x] = GetPixel(x + win->x, y + win->y);
        }
    }

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
    //DrawRect(window.x, window.y, window.w, window.h, window.color);
    DrawRoundedRect(window.x, window.y, window.w, window.h, 5, window.color);

    if (window.title)
    {
        SetCursorX(window.x + 10);
        SetCursorY(window.y +  4);

        Print(window.title, 0xFFFFFFFF);
    }

    DrawWindowButtons(window);

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

    //while (current && current->next)
    //{
    //    current = current->next;
    //}

    while (current)
    {
        DrawWindow(*current, 0);
        current = current->next;
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
    static int savedX = 0;
    static int savedY = 0;
    static int drawnBG = 0;

    if (pressed && !lastPressed)
    {
        WINDOW* win = GetWindowAt(mouseX, mouseY);

        if (win)
        {
            if (mouseY >= win->y && mouseY <= win->y + 20)
            {
                if (!drawnBG)
                {
                    //SaveBackgroundArea(win);
                    drawnBG = 1;
                }

                dragWindow = win;
                isDragging = 1;

                offsetX = mouseX - win->x;
                offsetY = mouseY - win->y;

                savedX = win->x;
                savedY = win->y;
                //SaveWindowArea(win);
                //;
                UpdateExplorer();

                BringToFront(win);
                activeWindow = win;
            }
        }
    }

    if (pressed && isDragging && dragWindow)
    {
        // Restaurar o fundo na posição anterior antes de mover
        dragWindow->x = savedX;
        dragWindow->y = savedY;
        RestoreWindowArea(dragWindow);

        // Mover para a nova posição
        dragWindow->x = mouseX - offsetX;
        dragWindow->y = mouseY - offsetY;

        if (dragWindow->x < 0) dragWindow->x = 0;
        if (dragWindow->y < 0) dragWindow->y = 0;
        if (dragWindow->x + dragWindow->w > WSCREEN)
            dragWindow->x = WSCREEN - dragWindow->w;
        if (dragWindow->y + dragWindow->h > HSCREEN)
            dragWindow->y = HSCREEN - dragWindow->h;

        // Salvar o fundo da nova posição
        savedX = dragWindow->x;
        savedY = dragWindow->y;
        SaveWindowArea(dragWindow);

        DrawWindow(*dragWindow, 0);
    }

    if (!pressed && lastPressed)
    {
        if (isDragging && dragWindow)
        {
            DrawAllWindows();
            isDragging = 0;
            dragWindow = NULL;
        }
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

    //Drawn on
    static int drawClock = 0;

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
                CreateWindow(1080, 440, 200, 250, 0xFF1A1A1A, "Clock");
                drawClock = 1;
            }
        }

        DrawAllWindows();
    }

    lastPressed = pressed;
}

void SaveBackgroundArea(WINDOW* win)
{
    if (!win->buffer)
    {    
        win->buffer = AllocateMemory(win->w * win->h * 4);
    }

    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            int zx = win->x + x;
            int zy = win->y + y;

            win->buffer[y * win->w + x] = GetPixel(zx, zy);
        }
    }
}

void SaveWindowArea(WINDOW* win)
{
    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            int screenX = win->x + x;
            int screenY = win->y + y;

            win->buffer[y * win->w + x] = GetPixel(screenX, screenY);
        }
    }
}

void RestoreWindowArea(WINDOW* win)
{
    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            int screenX = win->x + x;
            int screenY = win->y + y;

            SetPixel(screenX, screenY, win->buffer[y * win->w + x]);
        }
    }
}

void DrawButton(int x, int y, int size, const char* icon, DWORD color)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (icon[i * size + j] == '1')
            {
                SetPixel(x + j, y + i, color);
                SetPixel(x + j + 1, y + i, color);
            }
        }
    }
}

void CloseWindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    WINDOW* current = windowsList;

    while (current)
    {
        int buttonXStart = current->x + current->w - buttonSize - 10;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = current->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed &&
            x >= buttonXStart && x < buttonXEnd &&
            y >= buttonYStart && y < buttonYEnd)
        {
            // 🔥 remover da lista ligada

            if (current->prev)
                current->prev->next = current->next;
            else
                windowsList = current->next; // era a primeira

            if (current->next)
                current->next->prev = current->prev;

            // opcional: liberar memória
            FreeMemory(current->buffer);
            FreeMemory(current);

            UpdateExplorer();
            DrawAllWindows();
            return;
        }

        current = current->next;
    }
}

void ForceCloseWindow(WINDOW* win)
{
    if (win == NULL)
        return;

    if (win->prev)
        win->prev->next = win->next;
    else
        windowsList = win->next;

    if (win->next)
        win->next->prev = win->prev;

    if (win->buffer)
        FreeMemory(win->buffer);

    FreeMemory(win);

    UpdateExplorer();
    DrawAllWindows();
}

void DrawWindowButtons(WINDOW window)
{
    const int buttonSize = 10;

    const char closeIcon[] =
    {
        '1','0','0','0','1',
        '0','1','0','1','0',
        '0','0','1','0','0',
        '0','1','0','1','0',
        '1','0','0','0','1',
    };

    DrawButton(window.x + window.w - buttonSize - 10, window.y + 5, 5, closeIcon, 0xFFFFFFFF);

    const char maximizeIcon[] =
    {
        '1','1','1','1','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','0','0','0','1',
        '1','1','1','1','1',
    };

    DrawButton(window.x + window.w - (2 * buttonSize) - 20, window.y + 5, 5, maximizeIcon, 0xFFFFFFFF);

    const char minimizeIcon[] =
    {
        '0','0','0','0','0',
        '0','0','0','0','0',
        '1','1','1','1','1',
        '0','0','0','0','0',
        '0','0','0','0','0',
    };

    DrawButton(window.x + window.w - (3 * buttonSize) - 30, window.y + 5, 5, minimizeIcon, 0xFFFFFFFF);
}