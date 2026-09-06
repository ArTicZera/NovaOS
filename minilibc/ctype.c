#include "ctype.h"

int isalpha(int c) 
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isdigit(int c) 
{
    return (c >= '0' && c <= '9');
}

int isalnum(int c) 
{
    return isalpha(c) || isdigit(c);
}

int isspace(int c) 
{
    return (c == ' '  ||
            c == '\n' ||
            c == '\r' ||
            c == '\t' ||
            c == '\v' ||
            c == '\f');
}

int islower(int c) 
{
    return (c >= 'a' && c <= 'z');
}

int isupper(int c) 
{
    return (c >= 'A' && c <= 'Z');
}

int tolower(int c) 
{
    if (isupper(c))
    {
        return c + ('a' - 'A');
    }
    
    return c;
}

int toupper(int c) 
{
    if (islower(c))
    {
        return c - ('a' - 'A');
    }
    
    return c;
}
