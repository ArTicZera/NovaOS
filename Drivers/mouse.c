/*
   Coded by ArTic/JhoPro

   This driver is better to use in systems with a GUI, but if
   you tests the IMG file, I prefer enabling this driver on the
   C kernel launch.
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Graphics/graphics.h"
#include "../Interrupts/idt.h"
#include "../Font/text.h"
#include "../Timer/timer.h"
#include "../Userspace/GUI/gui.h"

#include "cursor.h"
#include "mouse.h"

//Drawing Buffers
DWORD MouseArea[WCURSOR * HCURSOR];
DWORD MouseCursorAfter[WCURSOR * HCURSOR];
DWORD MousePrevious[WCURSOR * HCURSOR];

//Mouse States
BYTE mouseData;
BYTE mouseCycle;
BYTE mousePacket[4];

int mousePressed = 0;

int prevMouseX = 0;
int prevMouseY = 0;

int mouseX = 0;
int mouseY = 0;

//Returns cursor X, Y and if its pressed
void GetMouseState(int* x, int* y, int* pressed) 
{
    *x = mouseX;
    *y = mouseY;
    *pressed = mousePressed;
}

//Draw the cursor
void DrawMouse(DWORD color)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        int x = 0;

        for (int i = 0; i < 2; i++)
        {
            BYTE byte = cursor[y * 2 + i];

            //Draw bit by bit
            for (int j = 7; j >= 0; j--)
            {
                //if bit = 0, continue
                //if bit = 1, set pixel
                if (byte & (1 << j))
                {
                    SetPixel(mouseX + x, mouseY + y, color);
                }
                x++;
            }
        }
    }
}

//Save behind the cursor
void SaveMouseArea()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            DWORD color = GetPixel(mouseX + x, mouseY + y);

            MouseArea[y * WCURSOR + x] = color;
        }
    }
}

//Draw behing the cursor
void RestorePreviousCursor()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            SetPixel(mouseX + x, mouseY + y, MouseArea[y * WCURSOR + x]);
        }
    }
}

//Waits for PS/2 to receive data
void MouseWait()
{
    while (inb(0x64) & 0x02);
}

//Clear cursor trails
void ClearTrails(int prevMouseX, int prevMouseY)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            SetPixel(prevMouseX + x, prevMouseY + y, MouseArea[y * WCURSOR + x]);
        }
    }
}

void MouseHandler()
{
   //Read PS/2
    mouseData = inb(0x60);

    //Packets sync
    if (mouseCycle == 0 && !(mouseData & 0x08))
    {
        return;
    }

    //Building a packet
    mousePacket[mouseCycle++] = mouseData;

    if (mouseCycle == 3)
    {
        mouseCycle = 0;

        mousePressed = (mousePacket[0] & 0x1);

        //Updates position
        int deltaX = (int)((char)mousePacket[1]);
        int deltaY = (int)((char)mousePacket[2]);

        //Save the previous position
        int prevMouseX = mouseX;
        int prevMouseY = mouseY;

        mouseX += deltaX;
        mouseY -= deltaY;

        //Make sure the cursor stays inside of the screen
        mouseX = (mouseX < 0) ? 0 : ((mouseX > WSCREEN - 16) ? WSCREEN - 16 : mouseX);
        mouseY = (mouseY < 0) ? 0 : ((mouseY > HSCREEN - 16) ? HSCREEN - 16 : mouseY);

        ClearTrails(prevMouseX, prevMouseY);
        SaveMouseArea();
        RestorePreviousCursor(prevMouseX, prevMouseY);
        DrawMouse(0xFFFFFFFF);
    }
}

void InitMouse()
{
    outb(0x64, 0xA7); //Disable auxilpiar PS/2 Device
    outb(0x64, 0xA8); //Enable Mouse

    while (inb(0x64) & 1)
    {
        inb(0x60);
    }

    //Setup Mouse IRQ
    IRQInstallHandler(12, &MouseHandler);

    outb(0x64, 0x20);              //Request config byte
    BYTE status = (inb(0x60) | 2); //Enable mouse interrupts
    outb(0x64, 0x60);              //Define byte config
    outb(0x60, status);

    outb(0x64, 0xD4); //Sends command to PS/2 Device
    outb(0x60, 0xF4); //Enable mouse data streaming
}
