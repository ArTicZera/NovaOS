#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Drivers/keyboard.h"
#include "../Font/text.h" 
#include "../Memory/mem.h"

#include "GUI/gui.h"
#include "login.h"

extern char user2[];

char userPass[16];
int loginLoop = 1;

void LoginScreen()
{
    DrawRect(0, 0, WSCREEN, HSCREEN, 0x13);
    
    int w = 300;
    int h = 150;

    int x = WSCREEN / 2 - w / 2;
    int y = HSCREEN / 2 - h / 2 - 50;

    y += h / 4;

    //Draw User Icon
    for (int ys = 0; ys < 32; ys++)
    {
        for (int xs = 0; xs < 32; xs++)
        {
            BYTE color = user2[ys * 32 + (xs + 8)];

            if (color != 0x00 && color != 28)
            {
                SetPixel(x + xs * 2, y + ys * 2, color);
                SetPixel(x + xs * 2 + 1, y + ys * 2, color);
                SetPixel(x + xs * 2, y + ys * 2 + 1, color);
                SetPixel(x + xs * 2 + 1, y + ys * 2 + 1, color);
            }
        }
    }

    SetCursorX(x + 70);
    SetCursorY(y + 20);

    Print("Hello, User!", 0x0F);

    SetCursorX(x);
    SetCursorY(y + 80);

    Print("Create a password", 0x0F);
    DrawRoundedRect(x, y + 100, 300, 20, 5, 0x12);

    SetCursorX(x + 5);
    SetCursorY(y + 100);

    while (loginLoop)
    {

    }
}

void SetPassword(char* pass)
{
    memcpy(userPass, pass, sizeof(userPass));

    loginLoop = 0x00;
}
