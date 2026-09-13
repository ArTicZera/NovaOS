WORD htons(WORD hostshort);
WORD ntohs(WORD netshort);

DWORD htonl(DWORD hostlong);
DWORD ntohl(DWORD netlong);

WORD CalculateChecksum(LPWORD addr, int length);
DWORD IPStringToDWORD(const char* ip);
