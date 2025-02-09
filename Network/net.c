#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Interrupts/idt.h"
#include "../Memory/alloc.h"
#include "../Memory/mem.h"
#include "../Hardware/pci.h"
#include "../Font/text.h"

#include "netutils.h"
#include "iptcp.h"
#include "icmp.h"
#include "net.h"

rtl8139 rtl8139Device;

static DWORD rxBuffer;
static DWORD iobase;

static BYTE currentTx = 0;

static int readptr;
static char transmitDesc;

static BYTE RTL8139SLOT;
static BYTE RTL8139BUS;

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
                    RTL8139BUS  = bus;
                    RTL8139SLOT = slot;

                    return bar & ~0x3;
                }
            }
        }
    }

    return 0x00;
}

void InitRTL8139()
{
    // Turn on RTL8139
    outb(iobase + 0x52, 0x00);

    // Software Reset
    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RESET);

    while ((inb(iobase + RTL8139_REG_COMMAND) & RTL8139_CMD_RESET) != 0x00) 
    { 
        // Wait for reset to complete
    }

    GetMAC();
    
    // Init Receive Buffer
    rxBuffer = (DWORD)AllocateMemory(8192 + 16);

    if (!rxBuffer)
    {
        Debug("Failed to Allocate Memory!", 0x01);
        return;
    }

    outl(iobase + RTL8139_REG_RX_ADDR_LOW, rxBuffer);

    // Enable interruptions
    outw(iobase + RTL8139_REG_INTR_STATUS, 0xFFFF);
    outb(iobase + RTL8139_REG_COMMAND, 0x0C);

    // Enable RX and TX
    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RX_EN | RTL8139_CMD_TX_EN);
}

void RTL8139SendPacket(void* packet, WORD length, BYTE* dstMac, WORD etherType)
{
    EthernetFrame ethFrame;
    memcpy(ethFrame.dstMac, dstMac, 6);
    memcpy(ethFrame.srcMac, GetMAC(), 6);
    ethFrame.etherType = htons(etherType);

    BYTE txBuffer[sizeof(EthernetFrame) + length];
    memcpy(txBuffer, &ethFrame, sizeof(EthernetFrame));
    memcpy(txBuffer + sizeof(EthernetFrame), packet, length);

    // Send the packet using the RTL8139 driver
    DWORD txAddr = RTL8139_REG_TX_ADDR_LOW + (currentTx * 4);
    DWORD txStatus = RTL8139_REG_TX_ADDR_HIGH + (currentTx * 4);

    memcpy((void*)inl(iobase + txAddr), txBuffer, sizeof(txBuffer));
    outl(iobase + txStatus, sizeof(txBuffer) & 0xFFFF);

    currentTx = (currentTx + 1) % 4;
}

int RTL8139ReceivePacket(LPBYTE buffer, int bufferLength)
{
    WORD packetLength = *(LPWORD)(rxBuffer + readptr + 2); 

    if (packetLength == 0 || packetLength > bufferLength) 
    {
        return 0;
    }

    memcpy(buffer, (void*)(rxBuffer + readptr), packetLength);

    readptr = (readptr + packetLength + 3) & (~3);

    outw(iobase + 0x38, readptr - 0x10);

    return packetLength;
}

void RTL8139Handler()
{
    WORD intrStatus = inw(iobase + RTL8139_REG_INTR_STATUS);

    if (intrStatus & 0x01) // Packet received
    {
        Debug("Packet Received!\n", 0x02);

        BYTE buffer[2048];
        int length = RTL8139ReceivePacket(buffer, sizeof(buffer));

        if (length > 0)
        {
            EthernetFrame* ethFrame = (EthernetFrame*)buffer;
            IPHeader* ipHdr = (IPHeader*)(buffer + sizeof(EthernetFrame));

            if (ntohs(ethFrame->etherType) == ETHERNET_IP)
            {
                if (ipHdr->protocol == 1) // ICMP
                {
                    ICMPHeader* icmpHdr = (ICMPHeader*)(buffer + sizeof(EthernetFrame) + sizeof(IPHeader));
                    HandleICMPReply(icmpHdr, length - sizeof(EthernetFrame) - sizeof(IPHeader));
                }
            }
        }
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

        BYTE irq = PCIConfigReadWord(RTL8139BUS, RTL8139SLOT, 0x00, 0x3C) & 0xFF;
        IRQInstallHandler(irq, &RTL8139Handler);
    }
}
