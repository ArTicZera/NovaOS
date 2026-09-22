#ifndef TASK_H
#define TASK_H

#define MAX_TASKS    16
#define TASK_UNUSED  0
#define TASK_READY   1
#define TASK_RUNNING 2
#define TASK_DEAD    3

typedef struct
{
    DWORD esp;
    DWORD ebp;
    DWORD eip;
    DWORD cr3;
    DWORD pid;
    DWORD st;
} Task;

void SchedulerInit(void);

int CreateTask(void(*entry)(void), DWORD stackTop);

DWORD Scheduler(DWORD currentESP);

Task* GetCurrentTask(void);

void ExitTask(void);

int TaskCount(void);

#endif
