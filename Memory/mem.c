/*
    Coded by ArTic/JhoPro

    The most important functions for an OS,
    since we need to manipulate data from memory
*/

#include "../Include/stdint.h"

#include "mem.h"

void* memset(void* dst, char val, DWORD num)
{
    LPBYTE tmp = (LPBYTE) dst;

    for (DWORD i = 0; i < num; i++)
    {
        tmp[i] = (BYTE) val;
    }

    return dst;
}

int strcmp(const char *str1, const char *str2) 
{
    while (*str1 && (*str1 == *str2)) 
    {
        str1++;
        str2++;
    }
    
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

DWORD strlen(const char *str) 
{
    DWORD length = 0;
    
    while (str[length] != '\0') 
    {
        length++;
    }
    
    return length;
}

char* strcat(char* dest, const char* src) 
{
    char* ptr = dest;

    while (*ptr != '\0') 
    {
        ptr++;
    }

    while (*src != '\0') 
    {
        *ptr = *src;
        ptr++;
        src++;
    }

    *ptr = '\0';

    return dest;
}

char* strncpy(char *dest, const char *src, DWORD n) 
{
    DWORD i = 0;
    
    while (i < n && src[i] != '\0') 
    {
        dest[i] = src[i];
        i++;
    }
    
    while (i < n) 
    {
        dest[i] = '\0';
        i++;
    }
    
    return dest;
}

int strncmp(const char *str1, const char *str2, DWORD n) 
{
    DWORD i = 0;

    while (i < n) 
    {
        if (str1[i] != str2[i]) 
        {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }

        if (str1[i] == '\0' || str2[i] == '\0') 
        {
            break;
        }

        i++;
    }

    return 0;
}

void* memcpy(void *dest, const void *src, DWORD n) 
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    
    while (n--) 
    {
        *d = *s;
        d++;
        s++;
    }
    
    return dest;
}

char* strcpy(char* dest, const char* src)
{
    char* original_dest = dest;

    while (*src != '\0') 
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return original_dest;
}

void* memmove(void* dest, const void* src, DWORD n) 
{
    LPBYTE d = (LPBYTE) dest;
    const LPBYTE s = (LPBYTE) src;

    if (d < s) 
    {
        for (DWORD i = 0; i < n; i++)
         {
            d[i] = s[i];
        }
    } 
    else 
    {
        for (DWORD i = n; i > 0; i--) 
        {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

char* strrchr(const char* str, int c)
{
    char* last = NULL;

    while (*str)
    {
        if (*str == (char)c)
        {
            last = (char*)str;
        }
        
        str++;
    }

    return last;
}
