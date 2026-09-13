#define ARP_REQUEST          0x01
#define ARP_REPLY            0x02
#define ARP_HW_TYPE_ETHERNET 0x01
#define ARP_PROTO_IP         0x0800
#define ARP_HW_ADDR_LEN      0x06
#define ARP_PROTO_ADDR_LEN   0x04
#define ARP_TABLE_SIZE       0x0A

typedef struct
{
    WORD  hwType;
    WORD  protoType;
    BYTE  hwAddrLen;
    BYTE  protoAddrLen;
    WORD  opcode;
    BYTE  senderHwAddr[ARP_HW_ADDR_LEN];
    DWORD senderProtoAddr;
    BYTE  targetHwAddr[ARP_HW_ADDR_LEN];
    DWORD targetProtoAddr;
} ARPHeader;

typedef struct
{
    DWORD ipAddress;
    BYTE  macAddress[ARP_HW_ADDR_LEN];
} ARPEntry;

void InitARP();
void ARPRequest(DWORD targetIp);
void ARPReply(ARPHeader *arpHdr);
BYTE ARPLookup(DWORD ip, LPBYTE mac);
void ARPReceivePacket(LPBYTE packet, int length);