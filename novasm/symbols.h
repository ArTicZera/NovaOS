#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef struct
{
    char name[64];
    unsigned int addr;
} Symbol;

extern Symbol symbols[256];
extern int symbolCount;

Symbol* FindSymbol(char* name);

#endif