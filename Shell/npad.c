#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Drivers/keyboard.h"
#include "../Timer/timer.h"
#include "../Memory/mem.h"
#include "../FileSystem/memfs.h"

#include "npad.h"

char* file;
int exitNotepad = 0;

void CreateNotepadFile(LPBYTE filename, LPBYTE buffer, int length)
{
    CreateFile(filename, buffer, length, PERM_R | PERM_W);
}

void ShowInfo(const char* filename)
{
    SetCursorX(0);
    SetCursorY(70 * 8);

    for (int x = 0; x < WSCREEN; x++)
    {
        for (int y = 70 * 8; y < 72 * 8; y++)
        {
            SetPixel(x, y, 0xFF00AA00);
        }
    }

    file = (char*)filename;

    Print("Editing file: ", 0xFFFFFFFF);

    if (strcmp(filename, "") == 0x00)
    {
        Print("Untitled", 0xFFFFFFFF);
        file = "Untitled";
    }

    Print(filename, 0xFFFFFFFF);

    SetCursorX(0);
    SetCursorY(72 * 8);

    Print("ESC - Exit    ^S - Save", 0xFFFFFF00);

    SetCursorX(0);
    SetCursorY(0);
}

void EscapeNotepad()
{
    exitNotepad = 0x01;
}

void StartNotepad(const char* filename)
{
    ClearScreen();

    ShowInfo(filename);

    KeyboardState(0x04);

    while (!exitNotepad)
    {
        KeyboardHandler();
    }

    KeyboardState(0x02);
}
