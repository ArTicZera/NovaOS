#ifndef IPC_H
#define IPC_H

#include "../Include/stdint.h"

#define IPC_MAX_MESSAGES   64
#define IPC_DATA_SIZE      8
#define IPC_MAX_PROCESSES  32

typedef enum
{
    IPC_NONE = 0,

    IPC_CREATE_WINDOW,
    IPC_CLOSE_WINDOW,
    IPC_MOVE_WINDOW,
    IPC_DRAW,
    IPC_PRESENT,

    IPC_KEYDOWN,
    IPC_KEYUP,

    IPC_MOUSEMOVE,
    IPC_MOUSEDOWN,
    IPC_MOUSEUP,

    IPC_CUSTOM

} IPCType;

typedef struct
{
    int sender;
    int receiver;

    IPCType type;

    DWORD data[IPC_DATA_SIZE];

} IPCMessage;

typedef struct
{
    IPCMessage messages[IPC_MAX_MESSAGES];

    int head;
    int tail;
    int count;

} IPCQueue;

void IPCInit(void);

int IPCSend(int receiver, IPCMessage* msg);

int IPCReceive(int receiver, IPCMessage* msg);

int IPCHasMessage(int receiver);

#endif