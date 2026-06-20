/*
    Coded by ArTic/JhoPro

    "Read Screen of Death"
*/

#include "../Include/stdint.h"
#include "../Graphics/graphics.h"
#include "../Font/text.h"
#include "../Interrupts/idt.h"

#include "panic.h"

void DeathScreen(const char* error)
{
    DrawRect(0, 0, WSCREEN, HSCREEN, 0xFFFF0000);

    SetCursorX(0x00);
    SetCursorY(0x00);

    Print("NovaOS has encountered a critical error and needs to shutdown!\n\n", 0xFFFFFFFF);

    Print("Error description: ", 0xFFFFFFFF);
    Print(error, 0xFFFFFFFF);

    Print("\n\nPossible causes:\n - An unexpected event disrupted kernel execution.\n - A critical system component failed to respond.\n - Illegal instructions or unauthorized memory access occurred.\n\nIf the problem persists, please contact the creator of the operating system via Discord: _articzera.\n\nRegisters:\n", 0xFFFFFFFF);
}
