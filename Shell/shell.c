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
    char args[2][16] = {{0}, {0}};
    int cmdIndex = 0;
    int inQuotes = 0;
    int argIndex = 0;
    int argCount = 0;
    int i = 0;

    //Copy its content
    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[cmdIndex++] = command[i++];
    }

    //Set last char to '\0'
    cmd[cmdIndex] = '\0';

    if (command[i] == ' ') i++;

    for (; command[i] != '\0'; i++)
    {
        if (command[i] == '\'') 
        {
            //Checks if it's inside of ''
            if (inQuotes) 
            {
                if (argCount < 2) 
                {
                    args[argCount][argIndex] = '\0';
                    argCount++;
                }
                argIndex = 0;
            }
            inQuotes = !inQuotes;
        } 
        else if (inQuotes && argCount < 2) 
        {
            args[argCount][argIndex++] = command[i];
        }
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
        Print("\n\nclear - ", 0x0A);  Print("Clear the screen.", 0x0F);
        Print("\nabout - ", 0x0A);    Print("More about NovaOS.", 0x0F);
        Print("\necho - ", 0x0A);     Print("Show a message on terminal.", 0x0F);
        Print("\nshutdown - ", 0x0A); Print("Shutdown the system.", 0x0F);
        Print("\nrestart - ", 0x0A);  Print("Reboot the system.", 0x0F);
        Print("\nneofetch - ", 0x0A); Print("Show system specs.", 0x0F);
        Print("\ngfx - ", 0x0A);      Print("Starts NovaOS GUI.", 0x0F);
        Print("\nmkfile - ", 0x0A);   Print("Create a file.", 0x0F);
        Print("\ncat - ", 0x0A);      Print("Read file content.", 0x0F);
        Print("\ninfo - ", 0x0A);     Print("Show a file info.", 0x0F);
        Print("\nrename - ", 0x0A);   Print("Rename a existing file.", 0x0F);
        Print("\ndel - ", 0x0A);      Print("Delete a existing file.", 0x0F);
        Print("\nls - ", 0x0A);       Print("List all files/dir in current local.", 0x0F);
        Print("\nmkdir - ", 0x0A);    Print("Create a directory.", 0x0F);
        Print("\ndeldir - ", 0x0A);   Print("Delete a existing directory.", 0x0F);
        Print("\ncd - ", 0x0A);       Print("Change the current directory.\n\n", 0x0F);
        Print("IMPORTANT!:" , 0x0C);  Print("To use arguments, place the symbol ''.", 0x0F);
    }
    else if (strcmp(cmd, "about") == 0x00)
    {
        Print("\n\nDeveloper: ", 0x0A);    Print("ArTic/JhoPro\n", 0x0F);
        Print("First Build Date: ", 0x0A); Print("01/05/2025\n\n", 0x0F);

        Print("This OS is being made for education purposes, to help students to how a OS works\n", 0x0F);
        Print("Also, NovaOS is the most hardest and challenging project I've coded before.\n", 0x0F);
        Print("A fun fact, this is my first OS with a GUI. I call it by VesaGFX.\n\n", 0x0F);

        Print("Special Credits: ", 0x0E); Print("Mist | Leo Ono | OS Dev Wiki", 0x0F);
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
        Print("                #             \n", 0x4C);
        Print(" #              ##            \n", 0x4C);
        Print(" ###           ###            \n", 0x4C);
        Print(" #####         ####           user@novaos-vm\n", 0x4C);
        Print(" #######      #####           --------------\n", 0x4C);
        Print(" ##########   ###### #######  OS: ", 0x4C); Print("NovaOS x86\n", 0x0F);
        Print(" ###  #######  ############   Kernel: ", 0x4C); Print("Alpha 1.4\n", 0x0F);
        Print(" ###     ###### #########     Resolution: ", 0x4C); Print("640x480\n", 0x0F);
        Print(" ###       ############       Video Mode: ", 0x4C); Print("VESA BIOS Extensions\n", 0x0F);
        Print(" ###        ##########        CPU: ", 0x4C); ShowCPUName();
        Print(" ###       ###########        Date: ", 0x4C); GetCMOSDate(); Print("\n", 0x00);
        Print(" ###      #####   #####       \n", 0x4C);
        Print("   #      ####      ###       \n", 0x4C);
        Print("         ##           ##        ", 0x4C);
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
            Print(buffer, 0x0F);
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
    else
    {
        Print("\n\nInvalid Command! Try again or use 'help' command.", 0x0C);
    }

    Print("\n\n", 0x00);
    PrintCurrentDir();
}

//Runs some ELF32 files
void ProcessShellRun(char* proccess)
{
    if (strcmp(proccess, "stars.exe") == 0x00)
    {
        ExecuteELF(stars);
    }
    else
    {
        Print("\n\nInvalid software!", 0x0C);
    }
}
