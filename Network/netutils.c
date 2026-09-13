#include "../Include/stdint.h"

#include "netutils.h"

WORD htons(WORD hostshort) 
{
    return (hostshort << 8) | (hostshort >> 8);
}

DWORD htonl(DWORD hostlong)
{
    return ((hostlong & 0x000000FF) << 24) |
           ((hostlong & 0x0000FF00) << 8)  |
           ((hostlong & 0x00FF0000) >> 8)  |
           ((hostlong & 0xFF000000) >> 24);
}

DWORD ntohl(DWORD netlong) 
{
    return ((netlong & 0x000000FF) << 24) |
           ((netlong & 0x0000FF00) << 8)  |
           ((netlong & 0x00FF0000) >> 8)  |
           ((netlong & 0xFF000000) >> 24);
}

WORD ntohs(WORD netshort)
{
    return (netshort >> 8) | (netshort << 8);
}

WORD CalculateChecksum(LPWORD addr, int length)
{
    DWORD sum = 0x00;

    while (length > 1)
    {
        sum += *addr++;
        length -= 2;
    }

    if (length == 1)
    {
        sum += *(LPBYTE) addr;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return (WORD)(~sum);
}

DWORD IPStringToDWORD(const char* ip)
{
    DWORD result = 0;
    int part = 0;

    while (*ip) 
    {
        if (*ip >= '0' && *ip <= '9') 
        {
            part = part * 10 + (*ip - '0');
        } 
        else if (*ip == '.') 
        {
            result = (result << 8) | (part & 0xFF);
            
            part = 0;
        }

        ip++;
    }

    result = (result << 8) | (part & 0xFF);

    return result;
}
