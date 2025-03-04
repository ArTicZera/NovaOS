#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "ata.h"

BYTE IdentifyATA(BYTE channel, BYTE drive)
{
    //Definition of the Primary/Secondary ports
    WORD base = (channel == 0) ? 0x1F0 : 0x170;

    //If drive == 0 -> Master, If drive == 1 -> Slave
    BYTE driveSelect = (drive == 0) ? 0xA0 : 0xB0;

    outb(base + 6, driveSelect);
    outb(base + 2, 0x00);
    outb(base + 3, 0x00);
    outb(base + 4, 0x00);
    outb(base + 5, 0x00);

    //ATA Identify Command
    outb(base + 7, 0xEC);

    BYTE status = inb(base + 7);
    if (status == 0x00)
    {
        Debug("Error: Drive does not exist!\n", 0x01);
        return 0x01;
    }

    while (inb(base + 7) & 0x80);

    if ((inb(base + 4) | inb(base + 5)) != 0)
    {
        Debug("Error: Drive is not ATA\n", 0x01);
        return 0x01;
    }

    while (!(inb(base + 7) & 0x08));

    if (inb(base + 7) & 0x01)
    {
        Debug("ATA Identify Error\n", 0x01);
        return 0x01;
    }
    else
    {
        Debug("Drive is ATA\n", 0x00);
    }

    return 0x00;
}

void ReadFromATA(BYTE channel, BYTE drive, DWORD lba, WORD* buffer)
{
    WORD base = (channel == 0) ? 0x1F0 : 0x170;

    outb(base + 6, 0xE0 | (drive << 4) | (BYTE)((lba >> 24) & 0x0F));

    outb(base + 1, 0x00);
    outb(base + 2, 0x01);
    outb(base + 3, (uint8_t)(lba));
    outb(base + 4, (uint8_t)(lba >> 8));
    outb(base + 5, (uint8_t)(lba >> 16));

    outb(base + 7, 0x20);

    while (!(inb(base + 7) & 0x08));

    for (int i = 0; i < 256; i++)
    {
        buffer[i] = inw(base);
        asm volatile("nop; nop; nop");
    }

    Print("Sector Readed Successfully!\n", 0x0A);
}

void WriteByteToATA(BYTE channel, BYTE drive, DWORD lba, BYTE data)
{
    WORD base = 0x170;

    WORD buffer[256] = { 0xFFFF };

    outb(base + 0x0C, 0x02);

    outb(base + 6, 0xE0 | (BYTE)((lba >> 24) & 0x0F));

    outb(base + 1, 0x00);
    outb(base + 2, 0x01);
    outb(base + 3, (uint8_t)(lba));
    outb(base + 4, (uint8_t)(lba >> 8));
    outb(base + 5, (uint8_t)(lba >> 16));

    outb(base + 7, 0x30);

    while (!(inb(base + 7) & 0x08));

    while (inb(base + 7) & 0x80);

    for (int i = 0; i < 256; i++)
    {
        outw(base, buffer[i]);
        asm volatile("nop; nop; nop");
    }

    outb(base + 7, 0xE7);

    Print("Setor Has Been Writted!\n", 0x0A);

    PrintHex(buffer[0], 0x0F);
}
