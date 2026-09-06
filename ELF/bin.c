/*
    Coded by ArTic/JhoPro

    Run a raw BIN file
*/

#include "../Memory/alloc.h"
#include "../Font/text.h"
#include "../FileSystem/memfs.h"
#include "../Memory/alloc.h"

#include "bin.h"

void LoadBIN(void* img)
{
    unsigned int esp_before, esp_after;

    asm volatile("mov %%esp, %0" : "=r"(esp_before));

    void (*entry)(void) = (void (*)(void))img;
    entry();

    asm volatile("mov %%esp, %0" : "=r"(esp_after));

    PrintHex(esp_before, 0xFF0000FF);
    PrintHex(esp_after, 0xFF0000FF);

    Print("DEPOIS", 0xFFFFFFFF);
}
