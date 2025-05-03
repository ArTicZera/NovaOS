/*
    Coded by ArTic/JhoPro

    Here I start every driver, table, memory, timer and show
    informations about the system, like the CMOS memory and CPU
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Interrupts/idt.h"
#include "../Timer/timer.h"
#include "../Memory/vmm.h"
#include "../Memory/alloc.h"
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

//ELF32 Programs
extern char stars[];
extern char scroll[];
extern char bytebeat[];

//Size of Programs
extern DWORD starsSize;
extern DWORD scrollSize;
extern DWORD bbeatSize;

void main(void)
{
    Debug("VESA 640x480 8BPP Started!\n", 0x00);

    Debug("Kernel loaded!\n", 0x00);

    InitGDT();
    Debug("GDT Initialized!\n", 0x00);
    Debug("TSS Initialized!\n", 0x00);
    
    SetupIDT();
    Debug("IDT Loaded!\n", 0x00);

    InitVirtualMemory();
    Debug("Virtual Memory Manager Started!\n", 0x00);

    InitFileSystem();
    MakeDir("shell");
    ChangeDir("shell");
    CreateFile("stars.exe", stars, starsSize);
    CreateFile("scroll.exe", scroll, scrollSize);
    CreateFile("bytebeat.exe", bytebeat, bbeatSize);
    //CreateFile("starvadegame.sj", starvade, vadeSize);
    ChangeDir("..");
    Debug("MemFS File System Loaded!\n", 0x00);

    InitTimer();
    Debug("Timer Started!\n", 0x00);

    InitKeyboard();
    Debug("Keyboard Started!\n", 0x00);

    //InitMouse();
    //SetCursorX(0x00);
    //Debug("Mouse init!\n", 0x00);

    InitMemory();
    Debug("Memory Initialized!\n", 0x00);

    InitEthernet();
    InitARP();
    Debug("Ethernet Started!\n", 0x00);

    SetupSoundBlaster();
    Debug("Sound Blaster 16 Initialized!\n", 0x00);

    IdentifyATA(0, 0); // Primary
    IdentifyATA(1, 0); // second

    ShowCMOSMem();
    ListDisks();
    ShowCPUInfo();
    ShowPCIDevices();
    
    Sleep(0);
    ClearScreen();
    DrawBootScr();

    InitMouse();
    SetCursorX(0x00);

    StartShellNoGUI();
    UserSpace();
}
