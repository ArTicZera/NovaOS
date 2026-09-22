#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "task.h"

static Task tasks[MAX_TASKS];

static int currentTask = -1;
static int taskCountValue = 0;

static DWORD nextPID = 1;

void SchedulerInit(void)
{
    int i;

    for (i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].esp = 0;
        tasks[i].ebp = 0;
        tasks[i].eip = 0;
        tasks[i].cr3 = 0;
        tasks[i].pid = 0;
        tasks[i].st = TASK_UNUSED;
    }

    currentTask = -1;
    taskCountValue = 0;
    nextPID = 1;
}

int CreateTask(void (*entry)(void), DWORD stackTop)
{
    int i;
    DWORD* stack;

    if (!entry) return -1;

    for (i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].st == TASK_UNUSED)  break;
    }

    if (i >= MAX_TASKS) return -1;

    stackTop &= ~0x0F;

    stack = (DWORD*)stackTop;

    *(--stack) = 0x00000202;   // EFLAGS
    *(--stack) = 0x00000008;   // CS
    *(--stack) = (DWORD)entry; // EIP

    *(--stack) = 0; // EAX
    *(--stack) = 0; // ECX
    *(--stack) = 0; // EDX
    *(--stack) = 0; // EBX
    *(--stack) = 0; // ESP
    *(--stack) = 0; // EBP
    *(--stack) = 0; // ESI
    *(--stack) = 0; // EDI

    *(--stack) = 0x10; // DS
    *(--stack) = 0x10; // ES
    *(--stack) = 0x10; // FS
    *(--stack) = 0x10; // GS

    tasks[i].esp = (DWORD)stack;
    tasks[i].ebp = (DWORD)stack;
    tasks[i].eip = (DWORD)entry;

    tasks[i].cr3 = 0;

    tasks[i].pid = nextPID++;

    tasks[i].st = TASK_READY;

    taskCountValue++;

    return tasks[i].pid;
}

DWORD Scheduler(DWORD currentESP)
{
    int oldTask;
    int nextTask;
    int i;

    if (currentTask < 0)
    {
        for (i = 0; i < MAX_TASKS; i++)
        {
            if (tasks[i].st == TASK_READY)
            {
                currentTask = i;
                tasks[i].st = TASK_RUNNING;

                return tasks[i].esp;
            }
        }
      
        return currentESP;
    }

    oldTask = currentTask;

    tasks[oldTask].esp = currentESP;

    if (tasks[oldTask].st == TASK_RUNNING)
        tasks[oldTask].st = TASK_READY;

    nextTask = oldTask;


    for (i = 0; i < MAX_TASKS; i++)
    {
        nextTask++;

        if (nextTask >= MAX_TASKS)
            nextTask = 0;

        if (tasks[nextTask].st == TASK_READY)
        {
            currentTask = nextTask;

            tasks[nextTask].st = TASK_RUNNING;

            return tasks[nextTask].esp;
        }
    }

    currentTask = oldTask;
    tasks[oldTask].st = TASK_RUNNING;

    return currentESP;
}

Task* GetCurrentTask(void)
{
    if (currentTask < 0)
        return 0;

    return &tasks[currentTask];
}

void ExitTask(void)
{
    if (currentTask < 0)
        return;

    tasks[currentTask].st = TASK_DEAD;

    taskCountValue--;
}

int TaskCount(void)
{
    return taskCountValue;
}
