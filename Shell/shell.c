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

#include "../ELF/elf.h"

#include "shell.h"

//ELF32 Executable
extern char stars[];
extern char scroll[];
extern char bytebeat[];

//Shows a welcome message
void PrintWelcomeMSG()
{
    SetCursorX(0x00);
    SetCursorY(0x00);

    Print("Welcome to NovaOS Shell! ", 0x0F);
    Print("(NO GUI)\n\n", 0x0C);
    Print("Type 'help' to start using the shell.\n", 0x0F);
    Print("Type 'gfx' to start using the GUI.\n\n", 0x0F);
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
        Print("\n -----------------------------------------------------------------------", 0x0B);
        Print("\n clear     ", 0x0A); Print("- Clear the screen", 0x0F);
        Print("\n about     ", 0x0A); Print("- More about emexOS", 0x0F);
        Print("\n echo      ", 0x0A); Print("- Show a message on terminal", 0x0F);
        Print("\n shut      ", 0x0A); Print("- Shutdown the system", 0x0F);
        Print("\n reboot    ", 0x0A); Print("- Reboot the system", 0x0F);
        Print("\n syscheck  ", 0x0A); Print("- Show system specs", 0x0F);
        Print("\n gfx       ", 0x0A); Print("- Starts emexOS GUI", 0x0F);
        Print("\n crfile    ", 0x0A); Print("- Create a file", 0x0F);
        Print("\n rfile     ", 0x0A); Print("- Read file content", 0x0F);
        Print("\n info      ", 0x0A); Print("- Show a file info", 0x0F);
        Print("\n rfile     ", 0x0A); Print("- Rename a existing file", 0x0F);
        Print("\n delfile   ", 0x0A); Print("- Delete a existing file", 0x0F);
        Print("\n ls        ", 0x0A); Print("- List all files/dir in current local", 0x0F);
        Print("\n crdir     ", 0x0A); Print("- Create a directory", 0x0F);
        Print("\n deldir    ", 0x0A); Print("- Delete a existing directory", 0x0F);
        Print("\n cd        ", 0x0A); Print("- Change the current directory", 0x0F);
        Print("\n -----------------------------------------------------------------------", 0x0B); 
    }
    else if (strcmp(cmd, "about") == 0x00)
    {
        Print("\n -----------------------------------------------------------------------", 0x0B); 
        Print("\n\nDeveloper: ", 0x0A);    Print("ArTic/JhoPro\n", 0x0F);
        Print("First Build Date: ", 0x0A); Print("01/05/2025\n\n", 0x0F);

        Print("This OS is being made for education purposes, to help students to how a OS works\n", 0x0F);
        Print("Also, NovaOS is the most hardest and challenging project I've coded before.\n", 0x0F);
        Print("A fun fact, this is my first OS with a GUI. I call it by VesaGFX.\n\n", 0x0F);

        Print("Special Credits: ", 0x0E); Print("Mist | Leo Ono | OS Dev Wiki", 0x0F);
        Print("\n -----------------------------------------------------------------------", 0x0B); 
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
    else if (strcmp(cmd, "shut") == 0x00)
    {
        Shutdown();
    }
    else if (strcmp(cmd, "reboot") == 0x00)
    {
        Restart();
    }
    else if (strcmp(cmd, "syscheck") == 0x00)
    {
        Print("\n\n", 0x0F);
        Print("                #             \n", 0x4C);
        Print(" #              ##            \n", 0x4C);
        Print(" ###           ###            \n", 0x4C);
        Print(" #####         ####           jon@novaos-vm\n", 0x4C);
        Print(" #######      #####           --------------\n", 0x4C);
        Print(" ##########   ###### #######  OS: ", 0x4C); Print("NovaOS x86\n", 0x0F);
        Print(" ###  #######  ############   Kernel: ", 0x4C); Print("Alpha 1.8.2\n", 0x0F);
        Print(" ###     ###### #########     Resolution: ", 0x4C); Print("800x600\n", 0x0F);
        Print(" ###       ############       Video Mode: ", 0x4C); Print("VESA BIOS Extensions\n", 0x0F);
        Print(" ###        ##########        CPU: ", 0x4C); ShowCPUName();
        Print(" ###       ###########        Date: ", 0x4C); GetCMOSDate(); Print("\n", 0x00);
        Print(" ###      #####   #####       \n", 0x4C);
        Print("   #      ####      ###       \n", 0x4C);
        Print("         ##           ##        ", 0x4C);
        //color test...
    }
    else if (strcmp(cmd, "gfx") == 0x00)
    {
        UserspaceState(0x01);
    }
    else if (strcmp(cmd, "crfile") == 0x00)
    {
        CreateFile(args[0], args[1], sizeof(args[1]));
    }
    else if (strcmp(cmd, "crdir") == 0x00)
    {
        MakeDir(args[0]);
    }
    else if (strcmp(cmd, "rfile") == 0x00)
    {
        BYTE buffer[128];
        DWORD size;

        int result = ReadFile(args[0], buffer, &size);
        
        if (result != -1)
        {
            Print("\n", 0x00);
            Print(buffer, 0x0F);
        }
    }
    else if (strcmp(cmd, "info") == 0x00)
    {
        FileInfo(args[0]);
    }
    else if (strcmp(cmd, "rfile") == 0x00)
    {
        RenameFile(args[0], args[1]);
    }
    else if (strcmp(cmd, "delfile") == 0x00)
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
        //SendPing(args[0]);
    }
    else if (strcmp(cmd, "colors") == 0x00)
    {
        Print("\n  color ", 0x0F); Print("Purple 0x3A", 0x3A); Print(" ; color ", 0x0F); Print("Gray 0x1A", 0x1A); Print("        ; color ", 0x0F); Print("Dark Blue 0x01", 0x01); Print(" ; color ", 0x0F); Print("Pale Purple 0x52\n", 0x52);
        Print("  color ", 0x0F);   Print("Green 0x0A", 0x0A); Print("  ; color ", 0x0F); Print("Teal 0x4A", 0x4A); Print("        ; color ", 0x0F); Print("White 0x0F", 0x0F); Print("     ; color ", 0x0F); Print("Lavender 0x38\n", 0x38);
        Print("  color ", 0x0F);   Print("Orange 0x2A", 0x2A); Print(" ; color ", 0x0F); Print("Dark Green 0x7A", 0x7A); Print("  ; color ", 0x0F); Print("Blue 0x09", 0x09); Print("      ; color ", 0x0F); Print("Light Red 0x3C\n", 0x3C);
        Print("  color ", 0x0F);   Print("Red 0x0C", 0x0C); Print("    ; color ", 0x0F); Print("Pink 0x0D", 0x0D); Print("        ; color ", 0x0F); Print("Yellow 0x0E", 0x0E); Print("    ; color ", 0x0F); Print("Brown 0xE9\n", 0xE9);      
    }
    else if (strcmp(cmd, "sDebug") == 0x00)
    {
        Print("\n 1aA ", 0x3A);  // Purple
        Print("2bB\n", 0x0A);  // Green
        Print(" 3cC ", 0x2A);  // Orange
        Print("4dD\n", 0x1A);  // Gray
        Print(" 5eE ", 0x4A);  // Teal
        Print("6fF\n", 0x7A);  // Dark Green
        Print(" 7gG ", 0x01);  // Dark Blue
        Print("8hH\n", 0x0F);  // White
        Print(" 9iI ", 0x09);  // Blue
        Print("-jJ\n", 0x52);  // Pale Purple
        Print(" ;kK ", 0x38);  // Lavender
        Print(":lL\n", 0x3C);  // Light Red

    }
    else
    {
        Print("\n :STOP !Invalid Command! Try again or use 'help' command!", 0x0C);
    }
    Print("\n", 0x00);
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
        Print("\n\nInvalid software!", 0x0C);
    }
}
