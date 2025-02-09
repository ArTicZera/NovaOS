#include "../Include/stdint.h"
#include "../Memory/mem.h"

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

    memcpy(ethFrame.dstMac, "\xFF\xFF\xFF\xFF\xFF\xFF", 0x06);
    memcpy(ethFrame.srcMac, "\x00\x0C\x29\x3D\x59\x0A", 0x06);
    ethFrame.etherType = htons(ETHERNET_IP);

    //Setup IP
    ipHdr.versionIhl = (4 << 4) | (5); // IPv4 e IHL
    ipHdr.totalLength= htons(sizeof(IPHeader) + sizeof(TCPHeader) + length);
    ipHdr.id = htons(54321); // Identificador
    ipHdr.ttl = 64; // TTL
    ipHdr.protocol = 6; // TCP
    ipHdr.srcIp = htonl(MY_IP);
    ipHdr.dstIp = htonl(dstIp);
    ipHdr.checksum = CalculateChecksum((LPWORD)&ipHdr, sizeof(IPHeader));

    //Setup TCP    
    tcpHdr.srcPort = htons(TCP_PORT);
    tcpHdr.dstPort = htons(dstPort);
    tcpHdr.seqNum = 0;
    tcpHdr.ackNum = 0;
    tcpHdr.offsetReserved = (5 << 4);
    tcpHdr.window = htons(5840);
    tcpHdr.checksum = CalculateChecksum((LPWORD)&tcpHdr, sizeof(TCPHeader));

    RTL8139SendPacket(&ethFrame, sizeof(ethFrame), &ipHdr, sizeof(ipHdr), &tcpHdr, sizeof(tcpHdr), data, length);
}

void HandleICMPReply(ICMPHeader* icmpHdr, int length)
{
    if (length < sizeof(ICMPHeader))
    {
        Debug("Invalid ICMP packet length!\n", 0x01);
        return;
    }

    if (icmpHdr->type == 0x00) // ICMP Echo Reply
    {
        Debug("Received ICMP Echo Reply!\n", 0x02);
    }
    else
    {
        Debug("Received ICMP packet with type: ", 0x02);
        Debug(itoa(icmpHdr->type, 10), 0x02);
        Debug("\n", 0x02);
    }
}