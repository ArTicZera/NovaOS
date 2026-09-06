#ifndef PCI_H
#define PCI_H

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct
{
    DWORD base;
    BYTE  isIO;
} PCIBar;


DWORD PCIConfigReadWord(BYTE bus, BYTE slot, BYTE func, BYTE offset);
BYTE PCIConfigReadByte(BYTE bus, BYTE slot, BYTE func, BYTE offset);
PCIBar ReadBarPCI(BYTE bus, BYTE slot, BYTE func, BYTE barIndex);
void ShowPCIDevices();

#endif
