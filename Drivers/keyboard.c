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
int winPressed = 0;
int rPressed = 0;

//Command buffer for shell and others.
char commandBuffer[50];
int commandLength = 0;

//Lower case characters in scan code order
const char* lowercase[] = {
    " ", " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "\b", " ", "q", "w", "e", "r", "t", "y", "u", "i",
    "o", "p", "[", "]", "\n", " ", "a", "s", "d", "f", "g", "h",
    "j", "k", "l", ";", "'", "`", " ", "\\", "z", "x", "c", "v",
    "b", "n", "m", ",", ".", "/", " ", "*", " ", " "
};

//The same with but in Upper case
const char* uppercase[] = {
    " ", " ", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
    "_", "+", "\b", " ", "Q", "W", "E", "R", "T", "Y", "U", "I",
    "O", "P", "{", "}", "\n", " ", "A", "S", "D", "F", "G", "H",
    "J", "K", "L", ":", "\"", "~", " ", "|", "Z", "X", "C", "V",
    "B", "N", "M", "<", ">", "?", " ", " ", " ", " "
};

void KeyboardState(int state)
{
    if (state == TRUE)
    {
        //For GUI
        enableText = 1;
    }
    else if (state == 2)
    {
        //For Shell
        enableText = 2;
    }
    else if (state == 0)
    {
        //Reserved
        enableText = 0;
    }
    else
    {
        //For Login page
        enableText = 3;
    }
}

void KeyboardHandler()
{
    //Read scan code
    uint8_t scan = inb(0x60);

    //Check if its pressed
    uint8_t isPress = !(scan & 0x80);
    scan &= 0x7F;

    if (allowInput)
    {
        switch (scan)
        {
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
                //If its for Shell, then process sh command
                if (isPress && enableText == 2)
                {
                    commandBuffer[commandLength] = '\0';
                    ProcessShellCMD(commandBuffer);
                    commandLength = 0;
                }
                //If its for Login, then save the password
                if (isPress && enableText == 3)
                {
                    commandBuffer[commandLength] = '\0';
                    SetPassword(commandBuffer);
                    commandLength = 0;
                }

                break;

            //Win
            case 0x5B:
            case 0x5C:
                winPressed = isPress;
                break;

            default:
                //If any other non-special key is being pressed, 
                //then we start to load characters in our buffer

                //This IF is for Login
                if (isPress && enableText == 3 && commandLength < 49)
                {
                    if (shift || caps)
                    {
                        commandBuffer[commandLength] = uppercase[scan][0];
                    }
                    else
                    {
                        commandBuffer[commandLength] = lowercase[scan][0];
                    }

                    //Important, we have to hide the password
                    PrintOut('*', 0x0F);

                    commandLength++;
                }

                //That if is more general, except for 3
                if (isPress && enableText != 3 && commandLength < 49)
                {
                    if (shift || caps)
                    {
                        commandBuffer[commandLength] = uppercase[scan][0];
                        PrintOut(commandBuffer[commandLength], 0x0F);
                        commandLength++;
                    }
                    else
                    {
                        commandBuffer[commandLength] = lowercase[scan][0];
                        PrintOut(commandBuffer[commandLength], 0x0F);
                        commandLength++;
                    }
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
