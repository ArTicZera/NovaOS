#include "../Memory/mem.h"
#include "protocol.h"

void WLInitMessage(WLMessage* msg, DWORD type)
{
    memset(msg, 0, sizeof(WLMessage));

    msg->header.type = type;
    msg->header.size = 0;
}

void WLSetPayload(WLMessage* msg, void* data, DWORD size)
{
    if(size > WL_MAX_PAYLOAD)
    {
        size = WL_MAX_PAYLOAD;
    }
    
    memcpy(msg->payload, data, size);

    msg->header.size = size;
}