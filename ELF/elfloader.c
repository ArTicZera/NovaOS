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
        if (programHeader[i].p_type == PTLOAD) 
        {
            DWORD virtualAddr = programHeader[i].p_vaddr;
            DWORD memSize = programHeader[i].p_memsz;
            DWORD fileSize = programHeader[i].p_filesz;
            DWORD offset = programHeader[i].p_offset;

            DWORD alignedAddr = ALIGN_DOWN(virtualAddr, PAGE_SIZE);
            DWORD alignedSize = ALIGN_UP(memSize, PAGE_SIZE);

            Debug("Mapping Segment\n", 0x00);
            Debug("Virtual Address: ", 0x00);
            PrintHex(virtualAddr, 0x0F);
            Print("\n", 0x00);

            if (!AllocateVirtualMemory(alignedAddr, alignedSize, 1, 1)) 
            {
                Debug("Failed to Allocate Virtual Memory\n", 0x01);
                return -1;
            }

            LPBYTE dest = (LPBYTE)TranslateAddress(virtualAddr);
            LPBYTE src  = (LPBYTE)elfData + offset;

            memcpy(dest, src, fileSize);

            if (memSize > fileSize)
            {
                memset(dest + fileSize, 0x00, memSize - fileSize);
            }

            if (programHeader[i].p_flags == 0)
            {
                memset(dest, 0, memSize);
            }

            if (programHeader[i].p_flags & 0x1) //Exec
            {
                Debug("Executable!\n", 0x02);
                SetMemoryProtection((DWORD)dest, alignedSize, PROT_EXEC | PROT_READ);
            }
            else if (programHeader[i].p_flags & 0x2) //Write
            {
                Debug("Write!\n", 0x02);
                SetMemoryProtection((DWORD)dest, alignedSize, PROT_WRITE | PROT_READ);
            }
            else //Read
            {
                Debug("Read!\n", 0x02);
                SetMemoryProtection((DWORD)dest, alignedSize, PROT_READ);
            }

            if (i == 0) 
            {
                relocationOffs = (DWORD)dest - virtualAddr;
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
