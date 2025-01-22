/*
    Coded by ArTic/JhoPro

    This was a pain in the ass to do. But as the functions says
    it loads and run an ELF32 file. (ONLY STATIC ELFs).

    Fortunately it's working perfectly now! :D
*/

#include "../Include/stdint.h"
#include "../Font/text.h"
#include "../Memory/mem.h"
#include "../Memory/vmm.h"
#include "../FileSystem/memfs.h"

#include "elf.h"

int LoadELF(void* elfData) 
{
    DWORD relocationOffs = 0;

    Print("\n", 0x00);
    Debug("Loading ELF...\n", 0x02);

    ELF32_Header* elfHeader = (ELF32_Header*)elfData;

    //Checks ELF header (0x7F E L F)
    if (*(DWORD*)elfHeader->e_ident != ELFMAGIC) 
    {
        Debug("Invalid ELF File\n", 0x01);
        return -1;
    }

    Debug("Valid ELF File!\n", 0x00);

    //Checks x86 architecture 
    if (elfHeader->e_machine != ELFARCH) 
    {
        Debug("Unsupported Architecture\n", 0x01);
        return -1;
    }

    Debug("Valid ELF Architecture!\n", 0x00);

    ELF32_ProgramHeader* programHeader = (ELF32_ProgramHeader*)((BYTE*)elfData + elfHeader->e_phoff);

    //Iterates by number of Program Headers, which are important
    //to load the sections of your program (.text, .data, .bss, etc)
    for (WORD i = 0; i < elfHeader->e_phnum; i++) 
    {
        //If isnt a PTLOAD type, continue to the next Program Header
        if (programHeader[i].p_type != PTLOAD) 
        {
            continue;
        }

        char* dest = (char*) programHeader[i].p_vaddr;   //Virtual Address
        char* src = elfData + programHeader[i].p_offset; //Program Offset
        DWORD filesz = programHeader[i].p_filesz;        //ELF File Size
        DWORD memsz = programHeader[i].p_memsz;          //Memory Size

        //Reserve memory for the ELF
        AllocateVirtualMemory((DWORD)dest, memsz, PROT_READ | PROT_WRITE | PROT_EXEC, 1);

        //.text, .data`
        if (filesz > 0) 
        {
            memmove(dest, src, filesz);
        }

        //.bss
        if (memsz > filesz) 
        {
            memset(dest + filesz, 0, memsz - filesz);
        }
    }

    //Offset entry
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
