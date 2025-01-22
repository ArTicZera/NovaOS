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
    address = (DWORD)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((DWORD)0x80000000));

    //Write Address
    outl(PCI_CONFIG_ADDRESS, address);
    
    //Read Data
    tmp = inl(PCI_CONFIG_DATA);
    
    return tmp;
}

void ShowPCIDevices()
{
    for (BYTE device = 0; device < 32; device++) 
    {
        for (BYTE func = 0; func < 8; func++) 
        {
            DWORD data = PCIConfigReadWord(0, device, func, 0);
            WORD VendorID = (uint16_t)(data & 0xFFFF);
            WORD DeviceID = (uint16_t)(data >> 16);

            if (VendorID != 0xFFFF) 
            {
                Debug("PCI Device Found at ", 2);
                PrintInt(device, 0x0F);
                Print(", ", 0x0F);
                PrintInt(func, 0x0F);
                Print(": Vendor ID = ", 0x0F);
                PrintHex(VendorID, 0x0F);
                Print(", Device ID = ", 0x0F);
                PrintHex(DeviceID, 0x0F);
                Print("\n", 0x0F);
            }
        }
    }
}
