#include "../Include/stdint.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../Memory/vmm.h"
#include "../FileSystem/memfs.h"

#include "elf.h"

int LoadELF(void* elfData) 
{
    DWORD relocationOffs = 0;
    DWORD physicalMem = 0;

    Print("\n", 0x00);
    Debug("Loading ELF...\n", 0x02);

    ELF32_Header* elfHeader = (ELF32_Header*)elfData;

    if (*(DWORD*)elfHeader->e_ident != ELFMAGIC) 
    {
        Debug("Invalid ELF File\n", 0x01);
        return -1;
    }

    Debug("Valid ELF File!\n", 0x00);

    if (elfHeader->e_machine != ELFARCH) 
    {
        Debug("Unsupported Architecture\n", 0x01);
        return -1;
    }

    Debug("Valid ELF Architecture!\n", 0x00);

    ELF32_ProgramHeader* programHeader = (ELF32_ProgramHeader*)((BYTE*)elfData + elfHeader->e_phoff);

    for (WORD i = 0; i < elfHeader->e_phnum; i++) 
    {
            if (programHeader[i].p_type != PTLOAD) 
            {
                continue;
            }

            char* dest = (char*) programHeader[i].p_vaddr;
            char* src = elfData + programHeader[i].p_offset;
            DWORD filesz = programHeader[i].p_filesz;
            DWORD memsz = programHeader[i].p_memsz;

            AllocateVirtualMemory((DWORD)dest, memsz, PROT_READ | PROT_WRITE | PROT_EXEC, 1);

            if (filesz > 0) 
            {
                memmove(dest, src, filesz);
            }

            if (memsz > filesz) 
            {
                memset(dest + filesz, 0, memsz - filesz);
            }
    }

    DWORD relocatedEntry = elfHeader->e_entry + relocationOffs;

    Debug("ELF Entry Point: ", 0x02);
    PrintHex(relocatedEntry, 0x0F);
    Print("\n", 0x00);

    void (*entryPoint)(void) = (void (*)(void))(relocatedEntry);
    entryPoint();

    return 0;
}

void ExecuteELF(void* elf)
{
    if (LoadELF(elf))
    {
        Debug("Failed to Load ELF File!\n", 0x01);
        return;
    }
}
