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