#include "../Include/stdint.h"
#include "../Font/text.h"

#include "af_unix.h"
#include "server.h"
#include "protocol.h"
#include "client.h"
#include "compositor.h"

static int serverSocket;
static int client = -1;

void WLServerInit()
{
    WLClientInit();

    serverSocket = socket(AF_UNIX, SOCK_STREAM);

    bind(serverSocket, "/run/wayland-0");

    listen(serverSocket);
}

void WLServerUpdate()
{
    int client = accept(serverSocket);

    if(client == -1) return;

    WLMessage msg;

    while(recv(client, &msg, sizeof(msg)))
    {
        //Print("RECEIVED()", 0xFFFFFFFF);
        HandleMessage(client, &msg);
    }
}