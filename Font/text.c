/*
    Coded by ArTic/JhoPro
    expanded by Jon

    Here we have some implementations of drawing the characters on 
    the screen, and some functions to draw strings, integers, and hex.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"

#include "font.h"

int cursorX = 0;
int cursorY = 0;
void ScrollScreen();
#define SCROLL_MARGIN_LINES 3
#define SCROLL_MARGIN (SCROLL_MARGIN_LINES * HFONT)

BYTE* framebuffer = (BYTE*)0xA0000;  // Beispieladresse für den VGA-Bereich (kann je nach System abweichen)


void DrawChar(BYTE* bitmap, BYTE color)
{
    int i = 0;

    for (int y = 0; y < HFONT; y++)
    {
        for (int x = WFONT - 1; x >= 0; x--)
        {
            // Lese jedes Bit und setze den Pixel
            if (bitmap[y] & (1 << x))
            {
                SetPixel(i + cursorX, y + cursorY, color);
            }

            i++;
        }

        i = 0;
    }

    // Bewege den Cursor nach rechts
    cursorX += 8;

    if (cursorX >= WSCREEN)
    {
        cursorX = 0;
        cursorY += HFONT;
    }
    
    if (cursorY + HFONT + SCROLL_MARGIN >= HSCREEN)
    {
        ScrollScreen(5000);
    }    
    
}

void Wait(int count)
{
    for (volatile int i = 0; i < count; i++) {
    }
}

void ScrollScreen(int delay)
{
    int scrollAmount = HFONT * 2;

    for (int y = scrollAmount; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            BYTE color = GetPixel(x, y);
            SetPixel(x, y - scrollAmount, color);
        }
        if (delay > 0)
            Wait(delay);
    }

    for (int y = HSCREEN - scrollAmount; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            SetPixel(x, y, 0x00);
        }
        if (delay > 0)
            Wait(delay);
    }

    cursorY -= scrollAmount;
    if (cursorY < 0)
        cursorY = 0;
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

//Just for debugging code
void Debug(const CHAR* str, int debug)
{
    switch (debug)
    {
        case 0:
            Print(" [+] ", 0x0A);    
            break;
        case 1:
            Print(" [-] ", 0x5D);
            break;
        case 2:
            Print(" [?] ", 0x0A);
            break;
        case 3:
            Print(" [ ] ", 0x0A);
            break;
        case 4:
            Print(" [sb16] ", 0x5D);
            break;
        case 5:
            Print(" [x] ", 0x0A);
            break;
    }

    Print(str, 0x0F);
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
void PrintInt(int value, BYTE color) 
{
    char buffer[11];
    IntToString(value, buffer);
    Print(buffer, color);
}

//Simple way to convert a value to HEX
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

//Thats our custom DrawChar, but with ASCII data
void PrintOut(char letter, BYTE color)
{
    DrawChar(isoFont + letter * HFONT, color);
}

//ASCII to Integer
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
