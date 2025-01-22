/*
    Coded by ArTic/JhoPro

    In this part we get more informations of the CPU like
    the vendor, name and manufacturer. Just by calling 'cpuid'
*/

#include "../Include/stdint.h"
#include "../Font/text.h"

#include "cpu.h"

char vendor[13];
char cpuname[49];

void GetVendor(char* vendor)
{
    int ebx, edx, ecx;

    asm volatile 
    (
        "xor %%eax, %%eax\n\t"
        "cpuid\n\t"
        : "=b" (ebx),
          "=d" (edx),
          "=c" (ecx)
        :
        : "eax"
    );

    *(int *)&vendor[0] = ebx;
    *(int *)&vendor[4] = edx;
    *(int *)&vendor[8] = ecx;
    vendor[12] = 0;
}

void GetCPUName(char* name)
{
    DWORD eax, ebx, ecx, edx;

    for (int i = 0; i < 3; i++) 
    {
        eax = 0x80000002 + i;

        asm volatile 
        (
            "cpuid"
            : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
            : "a" (eax)
        );

        *(unsigned int*)(name + i * 16 + 0) = eax;
        *(unsigned int*)(name + i * 16 + 4) = ebx;
        *(unsigned int*)(name + i * 16 + 8) = ecx;
        *(unsigned int*)(name + i * 16 + 12) = edx;
    }

    name[48] = '\0';
}

void ShowCPUName()
{
    Print(cpuname, 0x0F);
    Print("\n", 0x00);
}

void ShowCPUInfo()
{
    GetCPUName(cpuname);
    GetVendor(vendor);
   
    Debug("CPU Vendor: ", 2);
    Print(vendor, 0x0F);
    Print("\n", 0x00);

    Debug("CPU Name: ", 2);
    Print(cpuname, 0x0F);
    Print("\n", 0x00);
}
