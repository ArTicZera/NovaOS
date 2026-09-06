#ifndef SOCKET_H
#define SOCKET_H

#include "../Include/stdint.h"

#define AF_UNIX        1
#define SOCK_STREAM    1

#define MAX_SOCKETS    64

typedef struct
{
    int used;

    int fd;

    int domain;
    int type;

    int listening;

    int connected;

    int parent;

    int peer;

    char path[108];

    BYTE buffer[8192];

    int readPos;
    int writePos;

} UnixSocket;

int socket(int domain, int type);

int bind(int sock, const char *path);

int listen(int sock);

int accept(int sock);

int connect(int sock, const char *path);

int send(int sock, const void *buf, int len);

int recv(int sock, void *buf, int len);

int close(int sock);

int shutdown(int sock);

void SocketInit();

void SocketDump();

#endif