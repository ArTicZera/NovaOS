/*
    Coded by ArTic/JhoPro

    Extension by nicolasbickhoff11

    Here we have some implementations of drawing the characters on 
    the screen, and some functions to draw strings, integers, and hex.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Userspace/GUI/win.h"
#include "../Shell/shell.h"

#include "text.h" 
#include "font10x20.h"
#include "../Wayland/compositor.h"

#include "../Hardware/serial.h"

int cursorX = 0;
int cursorY = 0;

void DrawChar(BYTE* bitmap, DWORD color)
{
    for (int y = 0; y < HFONT; y++)
    {
        for (int x = 0; x < WFONT; x++)
        {
            int byte = x / 8;
            int bit  = 7 - (x % 8);

            if (bitmap[y * BYTES_PER_ROW + byte] & (1 << bit))
            {
                SetPixel(cursorX + x, cursorY + y, color);
            }
        }
    }

    cursorX += WFONT;

    if (cursorX >= WSCREEN)
    {
        cursorX = 0;
        cursorY += HFONT;
    }
    
}

extern int inGUI;

void DrawCharBuffer(DWORD* buffer, int w, int h, int x, int y, char c, DWORD color)
{
    unsigned char* bitmap = &isoFont[(unsigned char)c * HFONT * ((WFONT + 7) / 8)];

    for (int py = 0; py < HFONT; py++)
    {
        for (int px = 0; px < WFONT; px++)
        {
            int byte = px / 8;
            int bit  = 7 - (px % 8);

            if (bitmap[py * BYTES_PER_ROW + byte] & (1 << bit))
            {
                BufferSetPixel(buffer, w, h, x + px, y + py, color);
            }
        }
    }
}

void Print(const char* str, DWORD color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
	if (!inGUI)
            com1PutChar(str[i]);

        //If its '\n' goest to the next line.
        if (str[i] == '\n')
        {
            if (shellNOGUI)
            {
                cursorX = 0;
                cursorY += HFONT;
            }
            if (!shellNOGUI)
            {
                cursorX = winshellX;
                cursorY += HFONT;
                //shellNOGUI = 1;
            }

            continue;
        }

        if (str[i] == '\b')
        {
            if (cursorX > 0)
            {
                cursorX -= WFONT;
                DrawChar(isoFont + 0 * GLYPH_SIZE, 0);

                //Returns again because DrawChar update cursor
                // automatically
                cursorX -= WFONT;

            continue;
            }
        }

        if (str[i] == '\f')
        {
            DrawChar(isoFont + 0xDB * GLYPH_SIZE, color);

            continue;
        }

        //Look how I draw with the 'isoFont' bitmap (declared on font.h)
        //I use it with the size of a HFONT * the ASCII character, then
        //we get into the char bitmap to draw.
        //DrawChar(isoFont + str[i] * HFONT * (WFONT / 8), color);
        DrawChar(isoFont + (unsigned char)str[i] * GLYPH_SIZE, color);
    }
}

void fbPutChar(char character) 
{
    if (character == '\n')
    {
       if (shellNOGUI)
       {
           cursorX = 0;
           cursorY += HFONT;
       }
       if (!shellNOGUI)
       {
           cursorX = winshellX;
           cursorY += HFONT;
           //shellNOGUI = 1;
       }

       return;
    }
    if (character == '\b')
    {
        if (cursorX > 0)
        {
            cursorX -= WFONT;
            DrawChar(isoFont + 0 * GLYPH_SIZE, 0xFFFFFFFF);

            //Returns again because DrawChar update cursor
            // automatically
            cursorX -= WFONT;

            return;
        }
    }

    if (character == '\f')
    {
        DrawChar(isoFont + 0xDB * GLYPH_SIZE, 0xFFFFFFFF);

        return;
    }

    DrawChar(isoFont + (unsigned char)character * GLYPH_SIZE, 0xFFFFFFFF);
}

void PutChar(char character) 
{
    fbPutChar(character);
    com1PutChar(character);
}

void PrintBuffer(DWORD* buffer, int w, int h, int x, int y, const char* str, DWORD color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        //If its '\n' goest to the next line.
        if (str[i] == '\n')
        {
            if (shellNOGUI)
            {
                cursorX = 0;
                cursorY += HFONT;
            }
            if (!shellNOGUI)
            {
                cursorX = winshellX;
                cursorY += HFONT;
                //shellNOGUI = 1;
            }

            continue;
        }

        if (str[i] == '\b')
        {
            if (cursorX > 0)
            {
                cursorX -= WFONT;
                DrawCharBuffer(buffer, w, h, cursorX, cursorY, 0, 0);
                
                //Returns again because DrawChar update cursor
                // automatically
                cursorX -= WFONT;

            continue;
            }
        }

        if (str[i] == '\f')
        {
            DrawCharBuffer(buffer, w, h, cursorX, cursorY, 0xDB, color);
            //DrawCharBuffer(isoFont + 0xDB * GLYPH_SIZE, color);

            continue;
        }

        DrawCharBuffer(buffer, w, h, cursorX, cursorY, str[i], color);
    }
}

void DrawTextBuffer(DWORD *buffer, int w, int h, int x, int y, const char *text, DWORD color)
{
    int startX = x;

    while(*text)
    {
        if(*text == '\n')
        {
            x = startX;
            y += HFONT;
        }
        else
        {
            DrawCharBuffer(buffer, w, h, x, y, *text, color);
            x += WFONT;
        }

        text++;
    }
}

//Just for debugging code
void Debug(const CHAR* str, int debug)
{
    switch (debug)
    {
        case 0:
            Print("[  OK  ] ", 0xFF00FF00);    
            break;
        case 1:
            Print("[FAILED] ", 0xFFFF0000);
            break;
        case 2:
            Print("[ INFO ] ", 0xFF00FFFF);
            break;
    }

    Print(str, 0xFFFFFFFF);
}

//An implementation of 'itoa' from scratch
void IntToString(int value, char* buffer)
{
    char temp[11];
    int i = 0;
    int isNegative = 0;

    if (value < 0) 
    {
        isNegative = 1;
        value = -value;
    }

    do 
    {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    if (isNegative) 
    {
        temp[i++] = '-';
    }

    temp[i] = '\0';

    int j;

    for (j = 0; j < i; j++) 
    {
        buffer[j] = temp[i - j - 1];
    }

    buffer[j] = '\0';
}

//We just convert using IntToString and print the string
void PrintInt(int value, DWORD color) 
{
    char buffer[11];
    IntToString(value, buffer);
    Print(buffer, color);
}

//Simple way to convert a value to HEX
void PrintHex(int value, DWORD color)
{
    char buffer[9];

    Print("0x", color);

    for (int i = 7; i >= 0; i--) 
    {
        buffer[i] = "0123456789ABCDEF"[value & 0xF];
        value >>= 4;
    }

    buffer[8] = '\0';

    Print(buffer, color);
}
 
//Thats our custom DrawChar, but with ASCII data
void PrintOut(char letter, DWORD color)
{
    if (letter == '\b')
    {
        if (cursorX >= WFONT)
        {
            cursorX -= WFONT;

            DrawChar(isoFont + 0x00 * HFONT, 0x00000000);

            cursorX -= WFONT;
        }

        return;
    }

    if (letter == '\n')
    {
        cursorX = 0;
        cursorY += HFONT;
        return;
    }

    DrawChar(isoFont + letter * (HFONT * ((WFONT + 7) / 8)), color);
} 

//ASCII to Integer
static int atoi(const char* str) 
{
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ') 
    {
        i++;
    }

    if (str[i] == '-') 
    {
        sign = -1;
        i++;
    } 
    else if (str[i] == '+') 
    {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9')
     {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

void PrintHex32(DWORD value)
{
    char hex[] = "0123456789ABCDEF";

    for(int i = 28; i >= 0; i -= 4)
    {
        PrintOut(hex[(value >> i) & 0xF], 0xFFFFFFFF);
    }
}

void PrintByteHex(BYTE b)
{
    char hex[] = "0123456789ABCDEF";

    PrintOut(hex[b >> 4], 0xFFFFFFFF);
    PrintOut(hex[b & 0xF], 0xFFFFFFFF);
}

void MapFont()
{
    SetCursorX(winshellX);
    SetCursorY(GetCursorY() + HFONT);

    for (BYTE index = 0; index < 0xFF; index++)
    {
        PrintOut((BYTE)index, 0xFFFFFFFF);

        PrintOut(' ', 0x00);

        if ((index & 0x0F) == 0x0F)
        {
            PrintOut('\n', 0xFFFFFFFF);
        }
    }
}

//Here till the end consists of getting cursorX and cursorY
//for manipulation and set it to his own X and Y. Good for
//other .c files

int GetCursorX()
{
    return cursorX;
}

int GetCursorY()
{
    return cursorY;
}

void SetCursorX(int x)
{
    cursorX = x;
}

void SetCursorY(int y)
{
    cursorY = y;
}
