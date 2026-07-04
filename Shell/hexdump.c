#include "../Include/stdint.h"
#include "../Font/text.h"

#include "hexdump.h"

void HexDump(LPBYTE data, int size)
{
    Print("\n", 0x00);

    for(int i = 0; i < size; i += 16)
    {
        PrintHex32(i);
        Print("  ", 0x00);

        for(int j = 0; j < 16; j++)
        {
            if(i + j < size)
                PrintByteHex(data[i+j]);
            else
                Print("  ", 0x00);

            PrintOut(' ', 0xFFFFFFFF);

            if(j == 7)
                PrintOut(' ', 0xFFFFFFFF);
        }

        Print(" |", 0xFFFFFFFF);

        for(int j = 0; j < 16 && i+j < size; j++)
        {
            char c = data[i+j];

            if(c >= 32 && c <= 126)
            {
                PrintOut(c, 0xFFFFFFFF);
            }
            else
            {
                PrintOut('.', 0xFFFFFFFF);
            }
        }

        Print("\n", 0x00);
    }
}