#include "unistd.h"

void *sbrk(int increment)
{
    void *ret;

    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(45), "b"(increment)
    );

    return ret;
}

int open(const char *path, int flags)
{
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(5), "b"(path), "c"(flags)
    );
    return ret;
}

int close(int fd)
{
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(6), "b"(fd)
    );
    return ret;
}

int read(int fd, void *buf, size_t count)
{
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(3), "b"(fd), "c"(buf), "d"(count)
    );
    return ret;
}

int write(int fd, const void *buf, size_t count)
{
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(4), "b"(fd), "c"(buf), "d"(count)
    );
    return ret;
}

int lseek(int fd, int offset, int whence)
{
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(19), "b"(fd), "c"(offset), "d"(whence)
    );
    return ret;
}

/* exit simples */
void exit(int status)
{
    asm volatile (
        "int $0x80"
        :
        : "a"(1), "b"(status)
    );

    while (1) {}
}

int system(const char *cmd)
{
    (void)cmd;

    return -1;
}
