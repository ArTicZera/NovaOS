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
#include "../Userspace/GUI/win.h"
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
#include "../Shell/shell.h"
#include "../Userspace/GUI/gui.h"
#include "../Userspace/userspace.h"
#include "../GDT/gdt.h"

void main(struct multiboot_info* mbinfo, DWORD addr)
{
    struct vbe_mode_info_t* vbe = (struct vbe_mode_info_t*)mbinfo->vbe_mode_info;

    InitGraphics(vbe->framebuffer, vbe->pitch);
    Debug("VESA 640x480 32BPP Started!\n", 0x00);

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

    Debug("Kernel loaded!\n", 0x00);

    InitGDT();
    Debug("GDT Initialized!\n", 0x00);
    Debug("TSS Initialized!\n", 0x00);
    
    SetupIDT();
    Debug("IDT Loaded!\n", 0x00);

    InitVirtualMemory();
    Debug("Virtual Memory Manager Started!\n", 0x00);

    DWORD aligned = (mbinfo->mods_addr + 3) & ~3;
    struct multiboot_module_t* mods = (struct multiboot_module_t*)(DWORD)aligned;

    InitFileSystem();
    MakeDir("bin");
    ChangeDir("bin");

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

    //InitEthernet();
    //InitARP();
    //Debug("Ethernet Started!\n", 0x00);

    //SetupSoundBlaster();
    //Debug("Sound Blaster 16 Initialized!\n", 0x00);

    IdentifyATA(0, 0); // Primary Master
    IdentifyATA(1, 0); // Secondary Master

    ShowCMOSMem();
    ListDisks();
    ShowCPUInfo();
    ShowPCIDevices();

    Sleep(2);

    ClearScreen();
    DrawBootScr();

    StartShellNoGUI();
    UserSpace();
}

