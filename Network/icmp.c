#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "netutils.h"
#include "net.h"
#include "arp.h"
#include "iptcp.h"
#include "icmp.h"

void HandleICMPReply(ICMPHeader* icmpHdr, int length)
{
    if (length < sizeof(ICMPHeader))
    {
        return;
    }

    if (icmpHdr->type == 0x00) // ICMP Echo Reply
    {
        Debug("Received Echo Reply!\n", 0x02);
    }
}

void SendPing(const char* dstIp)
{
    Print("\n\n", 0x00);

    BYTE packet[ICMP_PACKET_SIZE];
    memset(packet, 0x00, ICMP_PACKET_SIZE);

    BYTE dstMac[6];
    DWORD dstIpDWORD = IPStringToDWORD(dstIp);

    if (!ARPLookup(dstIpDWORD, dstMac))
    {
        Debug("ARP Lookup failed! Sending ARP Request...\n", 0x02);
        ARPRequest(dstIpDWORD);

        int timeout = 1000000;
        while (!ARPLookup(dstIpDWORD, dstMac) && --timeout);

        if (timeout == 0x00)
        {
            Debug("ARP Request timed out!\n", 0x01);
            return;
        }
    }

    // Construct IP header
    IPHeader* ipHeader = (IPHeader*)packet;
    ipHeader->versionIhl = 0x45; // IPv4, 5 * 4 = 20 bytes header
    ipHeader->dscpEcn = 0;
    ipHeader->totalLength = htons(sizeof(IPHeader) + sizeof(ICMPHeader));
    ipHeader->id = htons(54321); // Random ID
    ipHeader->flagsOffset = 0;
    ipHeader->ttl = 64;
    ipHeader->protocol = 1; // ICMP
    ipHeader->checksum = 0;
    ipHeader->srcIp = htonl(MY_IP); // Replace with your source IP
    ipHeader->dstIp = htonl(dstIpDWORD);
    ipHeader->checksum = CalculateChecksum((LPWORD)ipHeader, sizeof(IPHeader));

    // Construct ICMP header
    ICMPHeader* icmpHeader = (ICMPHeader*)(packet + sizeof(IPHeader));
    icmpHeader->type = 8; // ICMP Echo Request
    icmpHeader->code = 0;
    icmpHeader->checksum = 0;
    icmpHeader->id = htons(1); // Random ID
    icmpHeader->sequence = htons(1); // Sequence number

    // Calculate ICMP checksum
    icmpHeader->checksum = CalculateChecksum((LPWORD)icmpHeader, sizeof(ICMPHeader));

    Debug("Sending ICMP Echo Request...\n", 0x02);

    // Send the packet
    RTL8139SendPacket(packet, sizeof(IPHeader) + sizeof(ICMPHeader), dstMac, ETHERNET_IP);
}
