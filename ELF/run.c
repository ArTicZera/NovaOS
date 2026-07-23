#include "../Include/stdint.h"
#include "../Font/text.h"
#include "../FileSystem/memfs.h"
#include "../Memory/alloc.h"

#include "elf.h"

extern Directory* currentDir;

void RunProgram(char* filename)
{
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '\n' || filename[i] == '\r')
        {
            Debug("Comparing with: [", 0x02);
            Print(currentDir->files[i].filename, 0xFF00FFFF);
            Print("]\n", 0xFF00FFFF);
            
            filename[i] = '\0';
            break;
        }
    }

    DWORD size;
    LPBYTE buffer = AllocateMemory(1024 * 1024);

    Print("\n", 0x00);
    Debug("Trying to open: [", 0x02);
    Print(filename, 0xFFFFFFFF);
    Print("]\n", 0xFFFFFFFF);

    if (ReadFile(filename, buffer, &size) != 0)
    {
        Debug("Searching for: [", 0x02);
        Print(filename, 0xFFFFFF00);
        Print("]\n", 0xFFFFFF00);
    }

    Debug("Magic: ", 0x02);
    PrintHex(*(DWORD*)buffer, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("File loaded!\n", 0x02);

    void (*entry)() = (void (*)()) LoadELF(buffer, 1);

    if (!entry)
    {
        Debug("Invalid ELF!\n", 0x01);
        return;
    }

    Debug("Entry: ", 0x02);
    PrintHex((DWORD)entry, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("Executing...\n", 0x02);

    entry();
}
