#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Interrupts/idt.h"
#include "../Memory/alloc.h"
#include "../Memory/mem.h"
#include "../Hardware/pci.h"
#include "../Font/text.h"

#include "net.h"

rtl8139 rtl8139Device;

static DWORD rxBuffer;
static DWORD iobase;

static int readptr;
static char transmitDesc;

BYTE* GetMAC()
{
    rtl8139Device.mac[0] = inb(iobase + 0x00);
    rtl8139Device.mac[1] = inb(iobase + 0x01);
    rtl8139Device.mac[2] = inb(iobase + 0x02);
    rtl8139Device.mac[3] = inb(iobase + 0x03);
    rtl8139Device.mac[4] = inb(iobase + 0x04);
    rtl8139Device.mac[5] = inb(iobase + 0x05);

    return rtl8139Device.mac;
}

DWORD RTL8139_FIND_DEVICE()
{
    for (BYTE bus = 0; bus < 256; bus++)
    {
        for (BYTE slot = 0; slot < 32; slot++)
        {
            DWORD vendorDevice = PCIConfigReadWord(bus, slot, 0, 0x00);

            if (vendorDevice == ((RTL8139_DEVICE_ID << 16) | RTL8139_VENDOR_ID))
            {
                DWORD bar = PCIConfigReadWord(bus, slot, 0, 0x10);

                if (bar & 0x01)
                {
                    return bar & ~0x3;
                }
            }
        }
    }

    return 0x00;
}

void InitRTL8139()
{
    //Turn on RTL8139
    outb(iobase + 0x52, 0x00);

    //Software Reset
    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RESET);

    while ((inb(iobase + RTL8139_REG_COMMAND) & RTL8139_CMD_RESET) != 0x00) 
    { 

    }

    GetMAC();
    
    //Init Receive Buffer
    rxBuffer = (DWORD)AllocateMemory(8192 + 16);

    if (!rxBuffer)
    {
        Debug("Failed to Allocate Memory!", 0x01);
        return;
    }

    outl(iobase + RTL8139_REG_RX_ADDR_LOW, rxBuffer);

    //Enable interruptions
    outw(iobase + RTL8139_REG_INTR_STATUS, 0xFFFF);
    outb(iobase + RTL8139_REG_COMMAND, 0x0C);

    //Enables RX and TX
    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RX_EN | RTL8139_CMD_TX_EN);

    transmitDesc = 0;
    readptr = 0;
    outl(iobase + RTL8139_REG_RX_ADDR_LOW, rxBuffer);
}

void ReceivePacket()
{
    WORD packetLength = *(LPWORD) (rxBuffer + readptr + 2);

    readptr = (readptr + packetLength + 7) & (~3);
    outw(iobase + 0x38, readptr - 0x10);
}

void RTL8139Handler()
{
    WORD intrStatus = inw(iobase + RTL8139_REG_INTR_STATUS);

    if (intrStatus & TER)
    {
        Debug("RTL8139 TER Set", 0x02);
    }
    if (intrStatus & RER)
    {
        Debug("RTL8139 RER Set", 0x02);
    }
    if (intrStatus & TOK)
    {
        //Transmition
    }
    if (intrStatus & ROK)
    {
        ReceivePacket();

        Debug("Packet Received!\n", 0x02);
    }

    outw(iobase + RTL8139_REG_INTR_STATUS, intrStatus);
}

void InitEthernet()
{
    iobase = RTL8139_FIND_DEVICE();
    
    if (iobase == 0x00)
    {
        Debug("RTL8139 NOT FOUND", 0x01);
    }
    else
    {
        InitRTL8139();
        Debug("RTL8139 Enabled!\n", 0x00);
        IRQInstallHandler(RTL8139_IRQ, &RTL8139Handler);
    }
}