#define ICMP_PACKET_SIZE 64

typedef struct
{
    BYTE type;
    BYTE code;
    WORD checksum;
    WORD id;
    WORD sequence;
} ICMPHeader;

void HandleICMPReply(ICMPHeader* icmpHdr, int length);
void SendPing(const char* dstIp);