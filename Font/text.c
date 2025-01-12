#include "../Include/stdint.h"
#include "../Graphics/graphics.h"

#include "font.h"

int cursorX = 0;
int cursorY = 0;

void DrawChar(BYTE* bitmap, BYTE color)
{
    int i = 0;

    for (int y = 0; y < HFONT; y++)
    {
        for (int x = WFONT - 1; x >= 0; x--)
        {
            if (bitmap[y] & (1 << x))
            {
                SetPixel(i + cursorX, y + cursorY, color);
            }

            i++;
        }

        i = 0;
    }

    cursorX += 8;

    if (cursorX >= WSCREEN)
    {
        cursorX = 0;
        cursorY += 16;
    }

}

void Print(const char* str, BYTE color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            cursorX = 0;
            cursorY += HFONT;

            continue;
        }

        if (str[i] == '\b')
        {
            if (cursorX != 0 && cursorY != 0)
            {
                cursorX -= 24;
                Print(" ", 0x00);
            }
        }

        DrawChar(isoFont + str[i] * HFONT, color);
    }
}

void Debug(const CHAR* str, int debug)
{
    switch (debug)
    {
        case 0:
            Print("[+] ", 0x0A);    
            break;
        case 1:
            Print("[-] ", 0x0C);
            break;
        case 2:
            Print("[?] ", 0x0B);
            break;
    }

    Print(str, 0x0F);
}

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

void PrintInt(int value, BYTE color) 
{
    char buffer[11];
    IntToString(value, buffer);
    Print(buffer, color);
}

void PrintHex(int value, BYTE color)
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

void PrintOut(char letter, BYTE color)
{
    DrawChar(isoFont + letter * HFONT, color);
}

int atoi(const char* str) 
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