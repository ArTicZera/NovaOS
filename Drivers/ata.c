/*
	Coded by MellOS and ArTic

	Interfaces used to connect storage devices
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Memory/alloc.h"
#include "../Font/text.h"

#include "ata.h"

#define STATUS_BSY      0x80
#define STATUS_RDY      0x40
#define STATUS_DRQ      0x08
#define STATUS_DF       0x20
#define STATUS_ERR      0x01

static inline void ata_delay_400ns(void) 
{
    inb(0x3F6);
    inb(0x3F6);
    inb(0x3F6);
    inb(0x3F6);
}

// TODO implement something on the lines of "took too long to respond"
void wait_BSY()
{
	while(inb(0x1F7) & STATUS_BSY){};
}

void wait_DRQ()
{
	while(!(inb(0x1F7) & STATUS_RDY)){};
}

BYTE check_ERR()
{
	return ((inb(0x1F7) & STATUS_ERR) != 0);
}

LPWORD identify_ata(BYTE drive)
{
	outb(0x1F6, drive);

	ata_delay_400ns();

	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);

	outb(0x1F7, 0xEC);

	ata_delay_400ns();
	
	BYTE status = inb(0x1F7);

	if (status == 0)
    {
		Debug("Error: drive does not exist\n", 0x01);
        return 0;
	}

	if (status == 0xFF)
    {
		Debug("Error: floating bus, there are no drives connected.\n", 0x01);
		return 0;
	}

	wait_BSY();

	if((inb(0x1F4) | inb(0x1F5)) != 0)
    {
		Debug("Error: drive is not ATA\n", 0x01);
		return 0;
	}

	wait_DRQ();

	if (check_ERR())
    {
		Debug("ATA Identify Error\n", 0x01);
		return 0;
	}

	LPWORD res = AllocateMemory(sizeof(LPWORD) * 256);

	for (int i = 0; i < 256; i++)
    {
		res[i] = inw(0x1F0);
	}

	return res;
}

void ata_print_error(BYTE error) 
{
    Debug("Error details: ", 0x01);

    if(error & 0x01) Print("AMNF ", 0xFFFFFFFF);  // Address Mark Not Found
    if(error & 0x02) Print("TK0NF ", 0xFFFFFFFF); // Track 0 Not Found
    if(error & 0x04) Print("ABRT ", 0xFFFFFFFF);  // Aborted Command
    if(error & 0x08) Print("MCR ", 0xFFFFFFFF);   // Media Change Request
    if(error & 0x10) Print("IDNF ", 0xFFFFFFFF);  // ID Not Found
    if(error & 0x20) Print("MC ", 0xFFFFFFFF);    // Media Changed
    if(error & 0x40) Print("UNC ", 0xFFFFFFFF);   // Uncorrectable Data
    if(error & 0x80) Print("BBK ", 0xFFFFFFFF);   // Bad Block

    Print("\n", 0x00);
}

void check_ata_error(void) 
{
    BYTE status = inb(0x1F7);

    if (status & 0x01) 
    {
        BYTE error = inb(0x1F1);
        Debug("ATA command error: status = ", 0x02);
        PrintHex(status, 0xFFFFFFFF);
        Print(", error = ", 0xFFFFFFFF);
        PrintHex(error, 0xFFFFFFFF);
        Print("\n", 0x00);
        ata_print_error(error);
    }
}

void LBA28_read_sector(BYTE drive, DWORD LBA, DWORD sector, LPWORD addr)
{
	identify_ata(drive);

	LBA = LBA & 0x0FFFFFFF;

    wait_BSY();

    outb(0x1F6, drive | ((LBA >> 24) & 0xF));

	ata_delay_400ns();

	outb(0x1F1, 0x00);
    outb(0x1F2, sector);
    outb(0x1F3, (BYTE) LBA);
    outb(0x1F4, (BYTE)(LBA >> 8));
	outb(0x1F5, (BYTE)(LBA >> 16)); 
	outb(0x1F7, 0x20); // 0x20 = 'Read' Command

	ata_delay_400ns();

	LPWORD tmp = addr;
	
    for (int j = 0; j < sector; j ++)
    {
		wait_BSY();
		wait_DRQ();
		
		for(int i = 0; i < 256; i++)
        {
            tmp[i] = inw(0x1F0);
        }

		tmp += 256;
	}

	check_ata_error();
}


void LBA28_write_sector(BYTE drive, DWORD LBA, DWORD sector, LPWORD buffer)
{
	identify_ata(drive);

	LBA = LBA & 0x0FFFFFFF;
	
	wait_BSY();

	outb(0x1F6, drive | ((LBA >> 24) & 0xF));		// send drive and bits 24 - 27 of LBA

	ata_delay_400ns();

	outb(0x1F1, 0x00);								// ?
	outb(0x1F2, sector);							// send number of sectors
	outb(0x1F3, (BYTE) LBA);						// send bits 0-7 of LBA
	outb(0x1F4, (BYTE) (LBA >> 8));				// 8-15
	outb(0x1F5, (BYTE) (LBA >> 16)); 			// 16-23
	outb(0x1F7,0x30); 								// 0x30 = 'Write' Command

	ata_delay_400ns();

	LPWORD tmp = buffer;
	
	for (int j = 0; j < sector; j++)
    {
		wait_BSY();
		wait_DRQ();
		
		for(int i = 0; i < 256; i++)
        {
			outw(0x1F0, tmp[i]);
		}

		tmp += 256;
	}

	check_ata_error();
}
