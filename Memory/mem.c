/*
    Coded by ArTic/JhoPro

    The most important functions for an OS,
    since we need to manipulate data from memory
*/

#include "../Include/stdint.h"

#include "alloc.h"
#include "mem.h"

void* memset(void* dst, char val, unsigned int num)
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

unsigned int strlen(const char *str) 
{
    unsigned int length = 0;
    
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

char* strncpy(char *dest, const char *src, unsigned int n) 
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

int strncmp(const char *str1, const char *str2, unsigned int n) 
{
    unsigned int i = 0;

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

void* memcpy(void *dest, const void *src, unsigned int n) 
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

void* memmove(void* dest, const void* src, unsigned int n) 
{
    LPBYTE d = (LPBYTE) dest;
    const LPBYTE s = (LPBYTE) src;

    if (d < s) 
    {
        for (unsigned int i = 0; i < n; i++)
         {
            d[i] = s[i];
        }
    } 
    else 
    {
        for (unsigned int i = n; i > 0; i--) 
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

int memcmp(const void *s1, const void *s2, unsigned int n) 
{
    const LPBYTE p1 = (const LPBYTE)s1;
    const LPBYTE p2 = (const LPBYTE)s2;

    for (DWORD i = 0; i < n; i++) 
    {
        if (p1[i] != p2[i]) 
        {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }

    return 0;
}

int strncasecmp(const char *s1, const char *s2, unsigned int n)
{
    unsigned char c1, c2;

    if (n == 0)
        return 0;

    while (n-- && *s1 && *s2) {
        c1 = *s1;
        c2 = *s2;

        // converte para minúsculo (A–Z)
        if (c1 >= 'A' && c1 <= 'Z')
            c1 = c1 - 'A' + 'a';

        if (c2 >= 'A' && c2 <= 'Z')
            c2 = c2 - 'A' + 'a';

        if (c1 != c2)
            return c1 - c2;

        s1++;
        s2++;

        if (n == 0)
            return 0;
    }

    if (n == 0)
        return 0;

    return *s1 - *s2;
}

int strcasecmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;

    while (*s1 && *s2) {
        c1 = *s1;
        c2 = *s2;

        if (c1 >= 'A' && c1 <= 'Z')
            c1 = c1 - 'A' + 'a';

        if (c2 >= 'A' && c2 <= 'Z')
            c2 = c2 - 'A' + 'a';

        if (c1 != c2)
            return c1 - c2;

        s1++;
        s2++;
    }

    return (unsigned char)*s1 - (unsigned char)*s2;
}

char *strchr(const char *s, int c)
{
    char ch = (char)c;

    while (*s) {
        if (*s == ch)
            return (char *)s;
        s++;
    }

    // permite buscar '\0'
    if (ch == '\0')
        return (char *)s;

    return 0;
}

char *strdup(const char *s)
{
    char *dup;
    unsigned int len = 0;
    unsigned int i;

    if (!s)
        return 0;

    // calcula tamanho
    while (s[len])
        len++;

    // aloca memória (+1 para '\0')
    dup = (char *)AllocateMemory(len + 1);
    if (!dup)
        return 0;

    // copia string
    for (i = 0; i <= len; i++)
        dup[i] = s[i];

    return dup;
}