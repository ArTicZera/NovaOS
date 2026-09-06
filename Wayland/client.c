#include "../Memory/mem.h"
#include "client.h"

static WLClient clients[WL_MAX_CLIENTS];

void WLClientInit()
{
    memset(clients, 0, sizeof(clients));
}

WLClient* WLAddClient(int socket)
{
    for(int i = 0; i < WL_MAX_CLIENTS; i++)
    {
        if(!clients[i].used)
        {
            clients[i].used = 1;
            clients[i].socket = socket;
            clients[i].id = i + 1;

            return &clients[i];
        }
    }

    return NULL;
}

WLClient* WLFindClient(int socket)
{
    for(int i = 0; i < WL_MAX_CLIENTS; i++)
    {
        if(clients[i].used && clients[i].socket == socket)
        {
            return &clients[i];
        }
    }

    return NULL;
}

void WLRemoveClient(int socket)
{
    WLClient* c = WLFindClient(socket);

    if(c) memset(c, 0, sizeof(WLClient));
}