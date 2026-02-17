/*
    Coded by ArTic/JhoPro and Daedalus

    A simple keyboard driver. It gets the scan code in 0x80 and
    it interprets as normal characters. It also set this to its
    IRQ (Interrupt Request), so everytime is checked if its pressed
*/

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Interrupts/idt.h"
#include "../Font/text.h"
#include "../Shell/shell.h"
#include "../Shell/npad.h"
#include "../Graphics/graphics.h"

#include "../Userspace/GUI/gui.h"
#include "../Userspace/userspace.h"
#include "../Userspace/login.h"

#include "keyboard.h"

int allowInput = 1; //Allows you to type something
int enableText = 0; //Enable text on screen

//Other keys events
int shift = 0;
int caps = 0;

//Command buffer for shell and others.
char commandBuffer[50];
int commandLength = 0;

char notepadBuffer[1024];
int notepadLength = 0;

//Lower case + Upper case, scan code
const char* keyMap[][2] = 
{
    { " ", " " }, { " ", " " },
    { "1", "!" }, { "2", "@" }, { "3", "#" }, { "4", "$" }, { "5", "%" },
    { "6", "^" }, { "7", "&" }, { "8", "*" }, { "9", "(" }, { "0", ")" },
    { "-", "_" }, { "=", "+" }, { "\b", "\b" }, { " ", " " },
    { "q", "Q" }, { "w", "W" }, { "e", "E" }, { "r", "R" }, { "t", "T" },
    { "y", "Y" }, { "u", "U" }, { "i", "I" }, { "o", "O" }, { "p", "P" },
    { "[", "{" }, { "]", "}" }, { "\n", "\n" }, { " ", " " },
    { "a", "A" }, { "s", "S" }, { "d", "D" }, { "f", "F" }, { "g", "G" },
    { "h", "H" }, { "j", "J" }, { "k", "K" }, { "l", "L" }, { ";", ":" },
    { "'", "\"" }, { "`", "~" }, { " ", " " }, { "\\", "|" },
    { "z", "Z" }, { "x", "X" }, { "c", "C" }, { "v", "V" }, { "b", "B" },
    { "n", "N" }, { "m", "M" }, { ",", "<" }, { ".", ">" }, { "/", "?" },
    { " ", " " }, { " ", " " }, { "*", " " }, { " ", " " }, { " ", " " }
};

void KeyboardState(int state)
{
    enableText = state;
}

static BYTE keyDown[128] = {0};

void HandleCharacter(int scan) 
{
    if (commandLength >= 49 || notepadLength >= 1024) return;

    char c = (shift || caps) ? keyMap[scan][1][0] : keyMap[scan][0][0];

    // Armazena no buffer
    commandBuffer[commandLength] = c;
    notepadBuffer[notepadLength] = c;

    // Imprime e incrementa **apenas uma vez**
    if (enableText == 3) // Senha
    {
        PrintOut('*', 0xFFFFFFFF);
        commandLength++;
        notepadLength++;
    }
    else if (enableText == 4) // Notepad
    {
        PrintOut(c, 0xFFFFFFFF);
        commandLength++;
        notepadLength++;
    }
    else if (enableText == 2) // Shell
    {
        PrintOut(c, 0xFFFFFFFF);
        commandLength++;
        notepadLength++;
    }
}

void KeyboardHandler()
{
    //Read scan code
    uint8_t scan = inb(0x60);

    //Check if its pressed
    uint8_t isPress = !(scan & 0x80);
    scan &= 0x7F;

    if (isPress)
    {
        if (keyDown[scan])
            return;
        keyDown[scan] = 1;
    }
    else
    {
        keyDown[scan] = 0;
        return;
    }

    if (allowInput)
    {
        switch (scan)
        {
            case 0x01:
                if (isPress)
                {
                    EscapeNotepad();
                }
                break;

            //Shift
            case 42:
            case 54:
                shift = isPress;
                break;

            //Caps lock
            case 58:
                if (isPress)
                {
                    caps = !caps;
                }
                break;

            //Enter
            case 0x1C:
                if (isPress) 
                {
                    commandBuffer[commandLength] = '\0';

                    if (enableText == 2) 
                    {
                        ProcessShellCMD(commandBuffer);
                    } 
                    else if (enableText == 3) 
                    {
                        SetPassword(commandBuffer);
                    }
                    else if (enableText == 4)
                    {
                        Print("\n", 0x00);
                    }

                    commandLength = 0;
                }

                break;

            //Win
            case 0x5B:
            case 0x5C:
                break;

            default:
                //If any other non-special key is being pressed, 
                //then we start to load characters in our buffer
                if (isPress) 
                {
                    HandleCharacter(scan);
                    break;
                }
                
                break;
        }
    }
}

void InitKeyboard()
{
    //Install the keyboard handler to an IRQ
    //Needed for the interrupt request
    IRQInstallHandler(0x01, &KeyboardHandler);
}
