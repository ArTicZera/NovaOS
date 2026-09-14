#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "af_unix.h"

UnixSocket sockets[MAX_SOCKETS];

void SocketInit()
{
    memset(sockets, 0, sizeof(sockets));
}

static int FindSocket(const char* path)
{
    for(int i = 0; i < MAX_SOCKETS; i++)
    {
        if(!sockets[i].used) continue;

        if(strcmp(sockets[i].path, path) == 0) return i;
    }

    return -1;
}

int socket(int domain,int type)
{
    for(int i = 0; i < MAX_SOCKETS; i++)
    {
        if(!sockets[i].used)
        {
            sockets[i].used = 1;

            sockets[i].fd = i;

            sockets[i].domain = domain;
            sockets[i].type = type;

            sockets[i].peer = -1;
            sockets[i].parent = -1;

            return i;
        }
    }

    return -1;
}

int bind(int sock, const char* path)
{
    if(sock < 0 || sock >= MAX_SOCKETS)
        return -1;

    strcpy(sockets[sock].path, path);

    return 0;
}

int listen(int sock)
{
    sockets[sock].listening = 1;

    return 0;
}

int connect(int sock,const char* path)
{
    int server = FindSocket(path);

    if(server==-1) return -1;

    int clientSide=-1;

    for(int i = 0; i < MAX_SOCKETS; i++)
    {
        if(!sockets[i].used)
        {
            clientSide = i;
            break;
        }
    }

    if(clientSide == -1)
        return -1;

    sockets[clientSide].used = 1;

    sockets[clientSide].connected = 1;

    sockets[clientSide].peer = sock;

    sockets[clientSide].parent = server;

    sockets[sock].peer = clientSide;

    sockets[sock].connected = 1;

    return 0;
}

int accept(int server)
{
    for(int i = 0; i < MAX_SOCKETS; i++)
    {
        if(!sockets[i].used) continue;

        if(sockets[i].parent == server && sockets[i].connected)
        {
            sockets[i].parent = -1;

            return i;
        }
    }

    return -1;
}

int send(int sock, const void* buf, int len)
{
    if(sock < 0) return -1;

    int peer = sockets[sock].peer;

    if(peer == -1) return -1;

    UnixSocket* p = &sockets[peer];

    if(len > sizeof(p->buffer) - p->writePos)
    {
        len=sizeof(p->buffer)-p->writePos;
    }

    memcpy(p->buffer + p->writePos, buf, len);

    p->writePos += len;

    return len;
}

int recv(int sock,void* buf,int len)
{
    UnixSocket* s = &sockets[sock];

    int available = s->writePos - s->readPos;

    if(available <= 0) return 0;

    if(len > available) len = available;

    memcpy(buf, s->buffer+s->readPos, len);

    s->readPos += len;

    if(s->readPos == s->writePos)
    {
        s->readPos = 0;
        s->writePos = 0;
    }

    return len;
}

int close(int sock)
{
    if(sock < 0) return -1;

    memset(&sockets[sock], 0, sizeof(UnixSocket));

    return 0;
}

int shutdown(int sock)
{
    sockets[sock].connected = 0;

    sockets[sock].peer = -1;

    return 0;
}

void SocketDump()
{
    for(int i = 0; i < MAX_SOCKETS; i++)
    {
        if(sockets[i].used)
        {
            Debug("Index: ", 0x02);
            PrintInt(i, 0xFFFFFFFF);
            Print(" Path = ", 0xFFFFFFFF);

            Print(sockets[i].path, 0xFFFFFFFF);
            Print(" Peer = ", 0xFFFFFFFF);

            PrintInt(sockets[i].peer, 0xFFFFFFFF);
            Print(" Parent = ", 0xFFFFFFFF);

            PrintInt(sockets[i].parent, 0xFFFFFFFF);
            Print(" Listening = ", 0xFFFFFFFF);

            PrintInt(sockets[i].listening, 0xFFFFFFFF);
            Print(" Connected = ", 0xFFFFFFFF);

            PrintInt(sockets[i].connected, 0xFFFFFFFF);
            Print(" readPos = ", 0xFFFFFFFF);

            PrintInt(sockets[i].readPos, 0xFFFFFFFF);
            Print(" writePos = ", 0xFFFFFFFF);

            PrintInt(sockets[i].writePos, 0xFFFFFFFF);
            Print("\n", 0xFFFFFFFF);
            
            Print("\n", 0x00);
            /*
            Debug("Socket: ", 0x02);
            PrintInt(i, 0xFFFFFFFF);
            Print("\n", 0x00);

            Debug("Path: ", 0x02);
            Print(sockets[i].path, 0xFFFFFFFF);
            Print("\n", 0x00);

            Debug("Listening: ", 0x02);
            PrintInt(sockets[i].listening, 0xFFFFFFFF);
            Print("\n", 0x00);

            Debug("Connected: ", 0x02);
            PrintInt(sockets[i].connected, 0xFFFFFFFF);
            Print("\n", 0x00);
            */
        }
    }
}