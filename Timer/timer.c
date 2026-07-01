/*
    Coded by ArTic/JhoPro

    Programmable Integrated Circuit IRQ implementation.
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Interrupts/idt.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "timer.h"

int ticks = 0;
int seconds = 0; 
int minutes = 0;
int drawBar = FALSE;

void PITIRQ0()
{
    ticks += 1;

    if (ticks % 20 == 0)
    {
        seconds++;
    }
}

unsigned int GetTicks()
{
    return ticks;
}

void Sleep(int seconds)
{
    int startTick = ticks;

    while (ticks < startTick + (startTick * (seconds * 10)))
    {
        
    }
}

void InitTimer(void)
{
    IRQInstallHandler(0x00, &PITIRQ0);

    DWORD divisor = 1193180 / 100;

    outb(0x43, 0x36);
    outb(0x40, (BYTE)(divisor & 0xFFFF));
    outb(0x40, (BYTE)((divisor & 0xFFFF) & 0xFF));
}
