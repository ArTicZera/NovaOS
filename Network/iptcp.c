#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "netutils.h"
#include "net.h"
#include "arp.h"
#include "iptcp.h"

void SendPacket(LPBYTE data, WORD length, DWORD dstIp, WORD dstPort)
{
    EthernetFrame ethFrame;
    IPHeader ipHdr;
    TCPHeader tcpHdr;

    memset(&ethFrame, 0x00, sizeof(ethFrame));
    memset(&tcpHdr, 0x00, sizeof(tcpHdr));
    memset(&ipHdr, 0x00, sizeof(ipHdr));

    // Resolve destination MAC address
    BYTE dstMac[6];
    if (!ARPLookup(dstIp, dstMac))
    {
        Debug("ARP Lookup failed! Sending ARP Request...\n", 0x02);
        ARPRequest(dstIp);

        int timeout = 1000000;
        while (!ARPLookup(dstIp, dstMac) && --timeout);

        if (timeout == 0x00)
        {
            Debug("ARP Request timed out!\n", 0x01);
            return;
        }
    }

    // Setup Ethernet frame
    memcpy(ethFrame.dstMac, dstMac, 6);
    memcpy(ethFrame.srcMac, GetMAC(), 6);
    ethFrame.etherType = htons(ETHERNET_IP);

    // Setup IP header
    ipHdr.versionIhl = (4 << 4) | (5); // IPv4 e IHL
    ipHdr.totalLength = htons(sizeof(IPHeader) + sizeof(TCPHeader) + length);
    ipHdr.id = htons(54321); // Identificador
    ipHdr.ttl = 64; // TTL
    ipHdr.protocol = 6; // TCP
    ipHdr.srcIp = htonl(MY_IP);
    ipHdr.dstIp = htonl(dstIp);
    ipHdr.checksum = CalculateChecksum((LPWORD)&ipHdr, sizeof(IPHeader));

    // Setup TCP header
    tcpHdr.srcPort = htons(TCP_PORT);
    tcpHdr.dstPort = htons(dstPort);
    tcpHdr.seqNum = 0;
    tcpHdr.ackNum = 0;
    tcpHdr.offsetReserved = (5 << 4);
    tcpHdr.window = htons(5840);
    tcpHdr.checksum = CalculateChecksum((LPWORD)&tcpHdr, sizeof(TCPHeader));

    // Combine headers and data into a single buffer
    BYTE txBuffer[sizeof(EthernetFrame) + sizeof(IPHeader) + sizeof(TCPHeader) + length];
    memcpy(txBuffer, &ethFrame, sizeof(EthernetFrame));
    memcpy(txBuffer + sizeof(EthernetFrame), &ipHdr, sizeof(IPHeader));
    memcpy(txBuffer + sizeof(EthernetFrame) + sizeof(IPHeader), &tcpHdr, sizeof(TCPHeader));
    memcpy(txBuffer + sizeof(EthernetFrame) + sizeof(IPHeader) + sizeof(TCPHeader), data, length);

    // Send the packet
    RTL8139SendPacket(txBuffer, sizeof(txBuffer), dstMac, ETHERNET_IP);
}
