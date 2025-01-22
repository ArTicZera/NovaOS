/*
    Coded by ArTic/JhoPro and Daedalus

    The CMOS (Complementary Metal-Oxide Semiconductor) is used to store
    basic informations about the system, like the memory size and date/time 
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "cmos.h"

USHORT GetCMOSMem()
{
    //Sends low memory in KB
    outb(0x70, 0x30);
    BYTE low = inb(0x71);

    //Sends high memory in KB
    outb(0x70, 0x31);
    BYTE high = inb(0x71);

    //Combine the lowest with the highest
    USHORT total = low | high << 8;

    return total;
}

void GetCMOSDate()
{
    //Access CMOS to get date
    outb(0x70, 0x0A);
    while(inb(0x71) & 0x80);

    //Gets the day
    outb(0x70, 0x07);
    BYTE day = inb(0x71);

    //Gets the month
    outb(0x70, 0x08);
    BYTE month = inb(0x71);

    //Gets the year
    outb(0x70, 0x09);
    BYTE year = inb(0x71);

    //Converts BCD to Decimal
    day   = ((day   / 16) * 10) + (day   & 0x0F);
    month = ((month / 16) * 10) + (month & 0x0F);
    year  = ((year  / 16) * 10) + (year  & 0x0F);

    PrintInt(month, 0x0F);
    Print("/", 0x0F);

    PrintInt(day, 0x0F);
    Print("/", 0x0F);

    PrintInt(year, 0x0F);
}

void ShowCMOSMem()
{
    Debug("Total CMOS Memory Size: ", 0x02);
    PrintHex(GetCMOSMem(), 0x0F);
    Print("\n", 0x00);
}
