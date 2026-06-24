#include "stdio.h"
#include "unistd.h"

void __assert_fail(const char *expr, const char *file, int line)
{
    printf("ASSERTION FAILED: %s\n", expr);
    printf("FILE: %s:%d\n", file, line);

    while (1)
    {
        asm volatile ("hlt");
    }
}
