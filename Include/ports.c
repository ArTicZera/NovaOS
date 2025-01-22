/*
    Coded by ??? 

    This are I/O with ports. I just don't remember who did this
*/

#include "stdint.h"

#include "ports.h"

void outb(WORD port, BYTE value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

char inb(WORD port)
{
    char ret;

    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    
    return ret;
}

void outw(WORD port, WORD value)
{
    asm volatile ("outw %0, (%1)" : : "a"(value), "d"(port));
}

WORD inw(WORD port)
{
    WORD ret;

    asm volatile ("inw (%1), %0" : "=a"(ret) : "d"(port));

    return ret;
}

void outl(WORD port, DWORD value)
{
    asm volatile ("outl %0, %1" : : "a" (value), "Nd" (port));
}

DWORD inl(WORD port)
{
    DWORD ret;

    asm volatile ("inl %1, %0" : "=a" (ret) : "Nd" (port));
    
    return ret;
}
