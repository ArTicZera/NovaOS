/*
    Symbols
*/

#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "symbols.h"

Symbol* FindSymbol(char* name)
{
    for (int i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbols[i].name, name) == 0)
        {
            return &symbols[i];
        }
    }

    return NULL;
}