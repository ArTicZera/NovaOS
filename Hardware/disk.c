/*
    Coded by ArTic/JhoPro

    Poorly implemented disk functions. Maybe it's inaccurate.
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "disk.h"

int WaitForReady(WORD base)
{
    for (int i = 0; i < 100000; i++)
    {
        BYTE status = inb(base + 7);

        //An empty channel floats to 0x00 or 0xFF, so there is nothing to wait for
        if (status == 0x00 || status == 0xFF)
        {
            return 0x00;
        }

        if (status & IDE_STATUS_ERROR)
        {
            return 0x00;
        }

        if ((status & IDE_STATUS_BUSY) == 0x00 && (status & IDE_STATUS_READY))
        {
            return 0x01;
        }
    }

    return 0x00;
}

DWORD GetDiskCapacity(WORD base)
{
    outw(base + 6, 0xA0);
    outw(base + 7, 0xEC);

    if (!WaitForReady(base))
    {
        return 0;
    }

    WORD data[256];

    for (int i = 0; i < 256; i++) 
    {
        data[i] = inw(base);
    }

    DWORD sectors = ((DWORD)data[61] << 16) | data[60];

    return sectors;
}

DWORD GetFloppyCapacity(void)
{
    BYTE status = inw(FLOPPY_STATUS_PORT);

    if (status & FLOPPY_READY)
    {
        return 1474560;
    }

    return 0;
}

void ListDisks(void)
{
    DWORD DiskCapacity;

    DiskCapacity = GetDiskCapacity(IDE_PRIMARY_COMMAND_PORT);
    
    if (DiskCapacity > 0)
    {
        Debug("Disk 0: ", 0x02);
        Print("Capacity: ", 0xFFFFFFFF);
        PrintHex(DiskCapacity, 0xFFFFFFFF);
        Print(" Sectors\n", 0xFFFFFFFF);
    }
    else
    {
        Debug("No Disk Found on Primary IDE Channel!\n", 0x01);
    }

    DWORD FloppyCapacity = GetFloppyCapacity();

    if (FloppyCapacity > 0)
    {
        Debug("Floppy Disk: ", 0x02);
        Print("Capacity: ", 0xFFFFFFFF);
        PrintInt(FloppyCapacity, 0xFFFFFFFF);
        Print(" Bytes\n", 0xFFFFFFFF);
    }
    else
    {
        Debug("No Floppy Disk found!\n", 0x01);
    }
}

