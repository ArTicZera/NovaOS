/*
    Coded by ArTic/JhoPro

    Programmable Integrated Circuit IRQ implementation.
    Real Time Clock added!
*/

#include "../Include/stdint.h"
#include "../Interrupts/idt.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "timer.h"

int ticks = 0;
int seconds = 0; 
int minutes = 0;
int drawBar = FALSE;

int hour, minute, second;

void PITIRQ0()
{
    ticks += 1;

    if (ticks % 20 == 0)
    {
        seconds++;
        RTCReadTime(&hour, &minute, &second);
    }

    if (ticks % 100 == 0)
    {
        
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

static int GetCMOSTime(BYTE reg)
{
    outb(0x70, reg);
    return inb(0x71);
}

static int BCDToBin(BYTE value)
{
    return (value & 0x0F) + ((value >> 4) * 10);
}

void RTCReadTime(int *hour, int *minute, int *second)
{
    BYTE sec;
    BYTE min;
    BYTE hr;
    BYTE regB;

    while (GetCMOSTime(0x0A) & 0x80);

    sec = GetCMOSTime(0x00);
    min = GetCMOSTime(0x02);
    hr  = GetCMOSTime(0x04);
    regB = GetCMOSTime(0x0B);

    if (!(regB & 0x04))
    {
        sec = BCDToBin(sec);
        min = BCDToBin(min);
        hr  = BCDToBin(hr & 0x7F) | (hr & 0x80);
    }

    if (!(regB & 0x02))
    {
        int isPM = hr & 0x80;
        hr &= 0x7F;

        if (!(regB & 0x04))
            hr = BCDToBin(hr);

        if (isPM)
        {
            if (hr != 12)
                hr += 12;
        }
        else
        {
            if (hr == 12)
                hr = 0;
        }
    }

    *hour   = hr;
    *minute = min;
    *second = sec;
}

void InitTimer(void)
{
    IRQInstallHandler(0x00, &PITIRQ0);

    DWORD divisor = 1193180 / 100;

    outb(0x43, 0x36);
    outb(0x40, (BYTE)(divisor & 0xFFFF));
    outb(0x40, (BYTE)((divisor & 0xFFFF) & 0xFF));
}
