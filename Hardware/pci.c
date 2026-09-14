/*
    Coded by ArTic/JhoPro and someone that I forgot (sorry xd)

    The PCI (Peripheral Component Interconnect) is kinda of hardware bus
    to conect peripheral devices, like GPUs, Sound and Network. In this
    code we read the PCI and shows the devices connected.
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "pci.h"

DWORD PCIConfigReadWord(BYTE bus, BYTE slot, BYTE func, BYTE offset)
{
    DWORD address;
    DWORD lbus  = (DWORD)bus;
    DWORD lslot = (DWORD)slot;
    DWORD lfunc = (DWORD)func;
    DWORD tmp = 0;

    //Config Address
    address = (DWORD)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((DWORD)0x80000000));

    //Write Address
    outl(PCI_CONFIG_ADDRESS, address);
    
    //Read Data
    tmp = inl(PCI_CONFIG_DATA);
    
    return tmp;
}

BYTE PCIConfigReadByte(BYTE bus, BYTE slot, BYTE func, BYTE offset)
{
    DWORD address;

    address = (1 << 31) | ((DWORD)bus << 16) | ((DWORD)slot << 11) | ((DWORD)func << 8) | (offset & 0xFC);

    outl(0xCF8, address);

    DWORD data = inl(0xCFC);

    return (BYTE)((data >> ((offset & 3) * 8)) & 0xFF);
}

PCIBar ReadBarPCI(BYTE bus, BYTE slot, BYTE func, BYTE barIndex)
{
    PCIBar bar = { 0 };

    BYTE offset = 0x10 + (barIndex * 4);
    DWORD barVal = PCIConfigReadWord(bus, slot, func, offset);

    if (barVal == 0)
    {
        return bar;
    }
    if (barVal & 0x1) 
    {
        bar.isIO = 1;
        bar.base  = barVal & ~0x3;
    } 
    else 
    {
        bar.isIO = 0;
        bar.base  = barVal & ~0xF;
    }

    return bar;
}

void ShowPCIDevices()
{
    for (BYTE device = 0; device < 32; device++) 
    {
        for (BYTE func = 0; func < 8; func++) 
        {
            DWORD data = PCIConfigReadWord(0, device, func, 0);
            WORD VendorID = (WORD)(data & 0xFFFF);
            WORD DeviceID = (WORD)(data >> 16);

            if (VendorID != 0xFFFF) 
            {
                Debug("PCI Device Found at ", 2);
                PrintInt(device, 0xFFFFFFFF);
                Print(", ", 0xFFFFFFFF);
                PrintInt(func, 0xFFFFFFFF);
                Print(": Vendor ID = ", 0xFFFFFFFF);
                PrintHex(VendorID, 0xFFFFFFFF);
                Print(", Device ID = ", 0xFFFFFFFF);
                PrintHex(DeviceID, 0xFFFFFFFF);
                Print("\n", 0xFFFFFFFF);
            }
        }
    }
}
