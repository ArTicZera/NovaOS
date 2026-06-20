/*
    Coded by ArTic/JhoPro
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"

#include "qemu.h"

//These will only work if you're using QEMU

void Shutdown()
{
    outw(0x604, 0x2000);
    while (1) asm("hlt");
}

void Restart()
{
    outb(0x64, 0xFE);
    while (1) asm("hlt");
}
