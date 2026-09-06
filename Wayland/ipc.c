#include "../Memory/mem.h"

#include "ipc.h"

static IPCQueue queues[IPC_MAX_PROCESSES];

void IPCInit(void)
{
    memset(queues, 0, sizeof(queues));
}

int IPCSend(int receiver, IPCMessage* msg)
{
    if (receiver < 0 || receiver >= IPC_MAX_PROCESSES)
        return 0;

    IPCQueue *queue = &queues[receiver];

    if (queue->count >= IPC_MAX_MESSAGES) return 0;

    queue->messages[queue->tail] = *msg;

    queue->tail++;

    if (queue->tail >= IPC_MAX_MESSAGES) queue->tail = 0;

    queue->count++;

    return 1;
}

int IPCReceive(int receiver, IPCMessage* msg)
{
    if (receiver < 0 || receiver >= IPC_MAX_PROCESSES)
        return 0;

    IPCQueue *queue = &queues[receiver];

    if (queue->count == 0) return 0;

    *msg = queue->messages[queue->head];

    queue->head++;

    if (queue->head >= IPC_MAX_MESSAGES)
    {
        queue->head = 0;
    }

    queue->count--;

    return 1;
}

int IPCHasMessage(int receiver)
{
    if (receiver < 0 || receiver >= IPC_MAX_PROCESSES) 
    {
        return 0;
    }

    return queues[receiver].count;
}