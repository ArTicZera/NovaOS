#ifndef WAYLAND_CLIENT_H
#define WAYLAND_CLIENT_H

#include "../Include/stdint.h"

#define WL_MAX_CLIENTS 32

typedef struct
{
    int used;

    int socket;

    DWORD id;

} WLClient;

void WLClientInit();

WLClient *WLAddClient(int socket);

WLClient *WLFindClient(int socket);

void WLRemoveClient(int socket);

#endif