#include "../Include/stdint.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../Memory/vmm.h"
#include "../FileSystem/memfs.h"

#include "elfloader.h"

int LoadELF(void* elfData) 
{
    DWORD relocationOffs = 0;
    DWORD physicalMem = 0;

    Print("\n", 0x00);
    Debug("Loading ELF...\n", 0x02);

    ELF32_Header* elfHeader = (ELF32_Header*)elfData;

    if (*(DWORD*)elfHeader->e_ident != ELFMAGIC) {
        Debug("Invalid ELF File\n", 0x01);
        return -1;
    }

    Debug("Valid ELF File!\n", 0x00);

    if (elfHeader->e_machine != ELFARCH) {
        Debug("Unsupported Architecture\n", 0x01);
        return -1;
    }

    Debug("Valid ELF Architecture!\n", 0x00);

    ELF32_ProgramHeader* programHeader = (ELF32_ProgramHeader*)((BYTE*)elfData + elfHeader->e_phoff);

    for (WORD i = 0; i < elfHeader->e_phnum; i++) 
    {
        if (programHeader[i].p_type == PTLOAD) 
        {
            DWORD virtualAddr = programHeader[i].p_vaddr;
            DWORD memSize = programHeader[i].p_memsz;
            DWORD fileSize = programHeader[i].p_filesz;
            DWORD offset = programHeader[i].p_offset;

            Debug("Mapping Segment\n", 0x00);
            Debug("Virtual Address: ", 0x00);
            PrintHex(virtualAddr, 0x0F);
            Print("\n", 0x00);

            if (!AllocateVirtualMemory(virtualAddr, memSize, 1, 1)) 
            {
                Debug("Failed to Allocate Virtual Memory\n", 0x01);
                return -1;
            }

            BYTE* dest = (BYTE*)TranslateAddress(virtualAddr);
            BYTE* src = (BYTE*)elfData + offset;

            memcpy(dest, src, fileSize);
            memset(dest + fileSize, 0x00, memSize - fileSize);

            if (i == 0) 
            {
                relocationOffs = (DWORD)dest - virtualAddr;
                physicalMem = relocationOffs + virtualAddr;
            }

            Debug("Segment Loaded at Physical Address: ", 0x00);
            PrintHex((DWORD)dest, 0x0F);
            Print("\n", 0x00);
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
