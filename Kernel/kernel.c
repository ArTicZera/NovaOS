/*
    Coded by ArTic/JhoPro

    Here I start every driver, table, memory, timer and show
    informations about the system, like the CMOS memory and CPU
*/

#include "../Include/stdint.h"
#include "multiboot.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Interrupts/idt.h"
#include "../Timer/timer.h"
#include "../Memory/vmm.h"
#include "../Memory/alloc.h"
#include "../Memory/mem.h"
#include "../Drivers/keyboard.h"
#include "../Drivers/mouse.h"
#include "../Drivers/sb16.h"
#include "../Drivers/ata.h"
#include "../Hardware/cpu.h"
#include "../Hardware/pci.h"
#include "../Hardware/disk.h"
#include "../Hardware/cmos.h"
#include "../Network/net.h"
#include "../Network/arp.h"
#include "../FileSystem/memfs.h"
//#include "../FileSystem/tarhdr.h"
#include "../Shell/shell.h"
//#include "../Programs/badapple.h"
#include "../Userspace/GUI/win.h"
#include "../Userspace/GUI/gui.h"
//#include "../Include/gif.h"
#include "../Userspace/userspace.h"
#include "../GDT/gdt.h"

/*
static DWORD OctalToInt(char* str)
{
    DWORD value = 0;

    while (*str >= '0' && *str <= '7')
    {
        value = (value * 8) + (*str - '0');
        str++;
    }

    return value;
}
*/

void main(struct multiboot_info* mbinfo, DWORD addr)
{
    struct vbe_mode_info_t* vbe = (struct vbe_mode_info_t*)mbinfo->vbe_mode_info;

    InitGraphics(vbe->framebuffer, vbe->pitch);
    Debug("VESA 800x600 32BPP Started!\n", 0x00);

    Debug("VESA Framebuffer: ", 0x00);
    PrintHex(GetFramebuffer(), 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("VESA Pitch: ", 0x00);
    PrintHex(GetPitch(), 0xFFFFFFFF);
    Print("\n", 0x00);

    if (addr != 0x2BADB002)
    {
        Debug("Invalid Magic Number!\n", 0x01);
        return;
    }

    Debug("Valid Magic Number!: 0x2BADB002\n", 0x00);

    /*
    for (int i = 0; i < mbinfo->mmap_length; i += sizeof(struct multiboot_mmap_entry))
    {
        struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*)(mbinfo->mmap_addr + i);

        Debug("Low Addr: ", 2);
        PrintHex(entry->addr_low, 0xFFFFFFFF);
        Print(" High Addr: ", 0xFFFFFFFF);
        PrintHex(entry->addr_high, 0xFFFFFFF);
        Print(" Low length: ", 0xFFFFFFFF);
        PrintHex(entry->len_low, 0xFFFFFFFF);
        Print(" High length: ", 0xFFFFFFFF);
        PrintHex(entry->len_high, 0xFFFFFFFF);
        Print("  Size: ", 0xFFFFFFFF);
        PrintInt(entry->size, 0xFFFFFFFF);
        Print(" Type: ", 0xFFFFFFFF);
        PrintInt(entry->type, 0xFFFFFFFF);
        Print("\n", 0x00);
    }
*/

    Debug("Kernel loaded!\n", 0x00);

    InitGDT();
    Debug("GDT Initialized!\n", 0x00);
    Debug("TSS Initialized!\n", 0x00);
    
    SetupIDT();
    Debug("IDT Loaded!\n", 0x00);

    InitVirtualMemory();
    Debug("Virtual Memory Manager Started!\n", 0x00);

    DWORD aligned = (mbinfo->mods_addr + 3) & ~3;
    //struct multiboot_module_t* mods = (struct multiboot_module_t*) mbinfo->mods_addr;
    struct multiboot_module_t* mods = (struct multiboot_module_t*)(DWORD)aligned;


    InitFileSystem();
    MakeDir("bin");
    ChangeDir("bin");

    /*
    BYTE* ptr = (BYTE*)mods[0].mod_start;

    while (1)
    {
        TarHeader* hdr = (TarHeader*)ptr;

        // fim do TAR
        if (hdr->name[0] == '\0')
            break;

        DWORD size = OctalToInt(hdr->size);

        // ignora diretórios
        if (hdr->typeflag == '0' || hdr->typeflag == '\0')
        {
                BYTE* data = ptr + 512;

                char* filename = hdr->name;

                // remove "./" do início
                if (filename[0] == '.' && filename[1] == '/')
                    filename += 2;

                Debug("Loading: ", 0x0F);
                Debug(filename, 0x0F);
                Debug("\n", 0x0F);

                if (size > 0)
                {
                    int r = CreateFile(
                        filename,
                        data,
                        size,
                        PERM_R | PERM_W | PERM_X
                    );

                    Debug("Result: ", 0x0F);
                    PrintInt(r, 0xFFFFFFFF);
                    Debug("\n", 0x0F);
                }
            }

            // próximo arquivo
            ptr += 512;
            ptr += ((size + 511) / 512) * 512;
    }*/

    
    for (int i = 0; i < mbinfo->mods_count; i++)
    {
        void* start = (void*) mods[i].mod_start;
        void* end   = (void*) mods[i].mod_end;
        DWORD size = (DWORD)((BYTE*)end - (BYTE*)start);

        char* filename = get_filename((char*)mods[i].string);
        
        CreateFile(filename, (LPBYTE)start, size, PERM_R | PERM_W | PERM_X);
    }
    
    
    ChangeDir("..");
    MakeDir("home");
    MakeDir("dev");
    MakeDir("tmp");
    Debug("MemFS File System Loaded!\n", 0x00);

    InitTimer();
    Debug("Timer Started!\n", 0x00);

    InitKeyboard();
    Debug("Keyboard Started!\n", 0x00);

    InitMouse();
    SetCursorX(0x00);
    Debug("Mouse Started!\n", 0x00);

    InitMemory();
    Debug("Memory Initialized!\n", 0x00);

    //PlayBadApple(badapple, badappleSize);

    SetupSoundBlaster();
    Debug("Sound Blaster 16 Initialized!\n", 0x00);

    ShowCMOSMem();
    ListDisks();
    ShowCPUInfo();
    ShowPCIDevices();

    InitEthernet();
 
    Sleep(2);

    ClearScreen();
    DrawBootScr();

    ClearScreen();
    StartShellNoGUI();
    UserSpace();
}
