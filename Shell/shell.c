/*
    Coded by ArTic/JhoPro

    This is a NO GUI Shell. It prints the current dir,
    process commands from 'keyboard.c' and parses to
    to run it.
*/

#include "../Include/stdint.h"
#include "../Drivers/keyboard.h"
#include "../Drivers/mouse.h"
#include "../Drivers/qemu.h"
#include "../Network/icmp.h"
#include "../Hardware/cpu.h"
#include "../Hardware/cmos.h"
#include "../Graphics/graphics.h"
#include "../Timer/timer.h"
#include "../Memory/mem.h"
#include "../FileSystem/memfs.h"
#include "../Font/text.h"
#include "../Userspace/GUI/gui.h"
#include "../Userspace/userspace.h"
#include "../Include/math.h"

#include "../tinygl/include/GL/gl.h"
#include "../tinygl/include/zbuffer.h"
#include "../Kernel/gfx/gears.h"

#include "../ELF/elf.h"

#include "shell.h"
#include "npad.h"

//ELF32 Executable
extern char stars[];
extern char scroll[];
extern char bytebeat[];

//Shows a welcome message
void PrintWelcomeMSG()
{
    SetCursorX(0x00);
    SetCursorY(0x00);

    Print("Welcome to NovaOS Shell! ", 0xFFFFFFFF);
    Print("(NO GUI)\n\n", 0xFFFF0000);
    Print("Type 'help' to start using the shell.\n", 0xFFFFFFFF);
    Print("Type 'gfx' to start using the GUI.\n\n", 0xFFFFFFFF);
}

//Print the first shell screen and
//Set keyboard state to 0x02.
void StartShellNoGUI()
{
    PrintWelcomeMSG();

    PrintCurrentDir();

    KeyboardState(0x02);
}

void ProcessShellCMD(char* command)
{
    char cmd[16] = {0};
    char args[2][16] = {{0}};
    int cmdIndex = 0;
    int argIndex = 0;
    int argCount = 0;
    int i = 0;

    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[cmdIndex++] = command[i++];
    }
    cmd[cmdIndex] = '\0';

    if (command[i] == ' ') i++;

    while (command[i] != '\0' && argCount < 2)
    {
        if (command[i] == ' ')
        {
            args[argCount][argIndex] = '\0';
            argCount++;
            argIndex = 0;
        }
        else
        {
            args[argCount][argIndex++] = command[i];
        }
        i++;
    }

    if (argIndex > 0 && argCount < 2)
    {
        args[argCount][argIndex] = '\0';
        argCount++;
    }

    if (GetCursorY() >= 448)
    {
        SetCursorX(0x00);
        SetCursorY(0x00);
        ClearScreen();
    }

    //Here starts the parser
    if (strcmp(cmd, "help") == 0x00)
    {
        Print("\n\nclear - ", 0xFF00FF00);  Print("Clear the screen.", 0xFFFFFFFF);
        Print("\nabout - ", 0xFF00FF00);    Print("More about NovaOS.", 0xFFFFFFFF);
        Print("\necho - ", 0xFF00FF00);     Print("Show a message on terminal.", 0xFFFFFFFF);
        Print("\nshutdown - ", 0xFF00FF00); Print("Shutdown the system.", 0xFFFFFFFF);
        Print("\nrestart - ", 0xFF00FF00);  Print("Reboot the system.", 0xFFFFFFFF);
        Print("\nneofetch - ", 0xFF00FF00); Print("Show system specs.", 0xFFFFFFFF);
        Print("\ngfx - ", 0xFF00FF00);      Print("Starts NovaOS GUI.", 0xFFFFFFFF);
        Print("\nmkfile - ", 0xFF00FF00);   Print("Create a file.", 0xFFFFFFFF);
        Print("\ncat - ", 0xFF00FF00);      Print("Read file content.", 0xFFFFFFFF);
        Print("\ninfo - ", 0xFF00FF00);     Print("Show a file info.", 0xFFFFFFFF);
        Print("\nrename - ", 0xFF00FF00);   Print("Rename a existing file.", 0xFFFFFFFF);
        Print("\ndel - ", 0xFF00FF00);      Print("Delete a existing file.", 0xFFFFFFFF);
        Print("\nls - ", 0xFF00FF00);       Print("List all files/dir in current local.", 0xFFFFFFFF);
        Print("\nmkdir - ", 0xFF00FF00);    Print("Create a directory.", 0xFFFFFFFF);
        Print("\ndeldir - ", 0xFF00FF00);   Print("Delete a existing directory.", 0xFFFFFFFF);
        Print("\ncd - ", 0xFF00FF00);       Print("Change the current directory.", 0xFFFFFFFF);
        Print("\nnpad - ", 0xFF00FF00);     Print("Terminal Notepad.", 0xFFFFFFFF);
        Print("\ngears - ", 0xFF00FF00);    Print("TinyGL Demonstration.", 0xFFFFFFFF);
    }
    else if (strcmp(cmd, "about") == 0x00)
    {
        Print("\n\nDeveloper: ", 0xFF00FF00);    Print("ArTic/JhoPro\n", 0xFFFFFFFF);
        Print("First Build Date: ", 0xFF00FF00); Print("01/05/2025\n\n", 0xFFFFFFFF);

        Print("This OS is being made for education purposes, to help students to how a OS works\n", 0xFFFFFFFF);
        Print("Also, NovaOS is the most hardest and challenging project I've coded before.\n", 0xFFFFFFFF);
        Print("A fun fact, this is my first OS with a GUI. I call it by VesaGFX.\n\n", 0xFFFFFFFF);

        Print("Special Credits: ", 0xFFFFFF00); Print("Mist | Leo Ono | Carbrito | OS Dev Wiki", 0xFFFFFFFF);
    }

    else if (strcmp(cmd, "clear") == 0x00)
    {
        SetCursorX(0x00);
        SetCursorY(0x00);
        ClearScreen();
    }
    else if (strcmp(cmd, "echo") == 0x00)
    {
        Print("\n\n", 0x00);
        Print(args[0], 0x0F);
    }
    else if (strcmp(cmd, "shutdown") == 0x00)
    {
        Shutdown();
    }
    else if (strcmp(cmd, "restart") == 0x00)
    {
        Restart();
    }
    else if (strcmp(cmd, "neofetch") == 0x00)
    {
        Print("\n\n", 0x0F);
        Print("                #             \n", 0xFF00FFFF);
        Print(" #              ##            \n", 0xFF00FFFF);
        Print(" ###           ###            \n", 0xFF00FFFF);
        Print(" #####         ####           root@novaos-vm\n", 0xFF00FFFF);
        Print(" #######      #####           --------------\n", 0xFF00FFFF);
        Print(" ##########   ###### #######  OS: ", 0xFF00FFFF); Print("NovaOS x86\n", 0xFFFFFFFF);
        Print(" ###  #######  ############   Kernel: ", 0xFF00FFFF); Print("Alpha 1.8.3\n", 0xFFFFFFFF);
        Print(" ###     ###### #########     Resolution: ", 0xFF00FFFF); Print("800x600\n", 0xFFFFFFFF);
        Print(" ###       ############       Video Mode: ", 0xFF00FFFF); Print("VESA BIOS Extensions\n", 0xFFFFFFFF);
        Print(" ###        ##########        CPU: ", 0xFF00FFFF); ShowCPUName();
        Print(" ###       ###########        Date: ", 0xFF00FFFF); GetCMOSDate(); Print("\n", 0x00);
        Print(" ###      #####   #####       \n", 0xFF00FFFF);
        Print("   #      ####      ###       \n", 0xFF00FFFF);
        Print("         ##           ##        ", 0xFF00FFFF);
    }
    else if (strcmp(cmd, "gfx") == 0x00)
    {
        UserspaceState(0x01);
    }
    else if (strcmp(cmd, "mkfile") == 0x00)
    {
        CreateFile(args[0], args[1], sizeof(args[1]));
    }
    else if (strcmp(cmd, "mkdir") == 0x00)
    {
        MakeDir(args[0]);
    }
    else if (strcmp(cmd, "cat") == 0x00)
    {
        BYTE buffer[128];
        DWORD size;

        int result = ReadFile(args[0], buffer, &size);
        
        if (result != -1)
        {
            Print("\n\n", 0x00);
            Print(buffer, 0xFFFFFFFF);
        }
    }
    else if (strcmp(cmd, "info") == 0x00)
    {
        FileInfo(args[0]);
    }
    else if (strcmp(cmd, "rename") == 0x00)
    {
        RenameFile(args[0], args[1]);
    }
    else if (strcmp(cmd, "del") == 0x00)
    {
        DeleteFile(args[0]);
    }
    else if (strcmp(cmd, "deldir") == 0x00)
    {
        DeleteDir(args[0]);
    }
    else if (strcmp(cmd, "ls") == 0x00)
    {
        ListFiles();
        ListDirs();
    }
    else if (strcmp(cmd, "cd") == 0x00)
    {
        ChangeDir(args[0]);
    }
    else if (strcmp(cmd, "run") == 0x00)
    {
        ProcessShellRun(args[0]);
    }
    else if (strcmp(cmd, "ping") == 0x00)
    {
        SendPing(args[0]);
    }
    else if (strcmp(cmd, "npad") == 0x00)
    {
        StartNotepad(args[0]);
    }
    else if (strcmp(cmd, "gears") == 0x00)
    {
        ZBuffer *zb = ZB_open(WSCREEN, HSCREEN, ZB_MODE_RGBA, (void*)GetFramebuffer());

        glInit(zb);

        init();
        reshape(WSCREEN, HSCREEN);

        INT i = 0;
        
        while (i < 100)
        {
            idle();

            i++;
        }

        ZB_close(zb);
        glEnd();
    }
    else
    {
        Print("\n\nInvalid Command! Try again or use 'help' command.", 0xFFFF0000);
    }

    Print("\n\n", 0x00);
    PrintCurrentDir();
}

//Runs some ELF32 files
void ProcessShellRun(char* process)
{
    if (strcmp(process, "stars.exe") == 0x00)
    {
        ExecuteELF(stars);
    }
    else if (strcmp(process, "scroll.exe") == 0x00)
    {
        ExecuteELF(scroll);
    }
    else if (strcmp(process, "bytebeat.exe") == 0x00)
    {
        ExecuteELF(bytebeat);
    }
    else
    {
        Print("\n\nInvalid software!", 0xFFFF0000);
    }
}
