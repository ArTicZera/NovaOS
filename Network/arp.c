#include "../Include/stdint.h"
#include "../Memory/mem.h"

#include "netutils.h"
#include "net.h"
#include "arp.h"

ARPEntry arpTable[ARP_TABLE_SIZE];
int arpTableSize = 0x00;

void InitARP()
{
    memset(arpTable, 0x00, sizeof(arpTable));
}

void ARPRequest(DWORD targetIp)
{
    ARPHeader arpHdr;

    arpHdr.hwType = htons(ARP_HW_TYPE_ETHERNET);
    arpHdr.protoType = htons(ARP_PROTO_IP);
    arpHdr.hwAddrLen = ARP_HW_ADDR_LEN;
    arpHdr.protoAddrLen = ARP_PROTO_ADDR_LEN;
    arpHdr.opcode = htons(ARP_REQUEST);

    arpHdr.senderProtoAddr = htonl(0xC0A80001); //192.168.0.1
    arpHdr.targetProtoAddr = htonl(targetIp);

    memcpy(arpHdr.senderHwAddr, "\x00\x0C\x29\x3D\x59\x0A", ARP_HW_ADDR_LEN);
    memset(arpHdr.targetHwAddr, 0x00, ARP_HW_ADDR_LEN);

    // Send ARP request to broadcast address
    RTL8139SendPacket(&arpHdr, sizeof(arpHdr), "\xFF\xFF\xFF\xFF\xFF\xFF", 0x01);
}

void ARPReply(ARPHeader *arpHdr)
{
    for (int i = 0; i < arpTableSize; i++) 
    {
        if (arpTable[i].ipAddress == ntohl(arpHdr->senderProtoAddr)) 
        {
            return;
        }
    }

    if (arpTableSize < ARP_TABLE_SIZE)
    {
        ARPEntry* entry = &arpTable[arpTableSize++];

        entry->ipAddress = ntohl(arpHdr->senderProtoAddr);
        memcpy(entry->macAddress, arpHdr->senderHwAddr, ARP_HW_ADDR_LEN);
    }
}

BYTE ARPLookup(DWORD ip, LPBYTE mac)
{
    for (int i = 0; i < arpTableSize; i++)
    {
        if (arpTable[i].ipAddress == ip)
        {
            memcpy(mac, arpTable[i].macAddress, ARP_HW_ADDR_LEN);
            return TRUE;
        }
    }

    return FALSE;
}

void ARPReceivePacket(LPBYTE packet, int length)
{
    if (length < sizeof(ARPHeader))
    {
        // Too short packet
        return;
    }

    ARPHeader* arpHdr = (ARPHeader*) packet;

    if (ntohs(arpHdr->hwType) != ARP_HW_TYPE_ETHERNET || ntohs(arpHdr->protoType) != ARP_PROTO_IP)
    {
        // Hardware or protocol type not supported
        return;
    }

    if (ntohs(arpHdr->opcode) == ARP_REQUEST || ntohs(arpHdr->opcode) == ARP_REPLY)
    {
        ARPReply(arpHdr);
    }
}
