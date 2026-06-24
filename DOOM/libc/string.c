#include "string.h"
#include "stdlib.h"
#include "ctype.h"

void *memcpy(void *dest, const void *src, size_t n) 
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    for (size_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t n) 
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (d < s) 
    {
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    } 
    else 
    {
        for (size_t i = n; i != 0; i--)
            d[i - 1] = s[i - 1];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) 
{
    unsigned char *p = s;

    for (size_t i = 0; i < n; i++)
    {
        p[i] = (unsigned char)c;
    }

    return s;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *a = s1;
    const unsigned char *b = s2;

    for (size_t i = 0; i < n; i++) 
    {
        if (a[i] != b[i])
        {
            return a[i] - b[i];
        }
    }

    return 0;
}

size_t strlen(const char *s) 
{
    size_t len = 0;

    while (s[len])
    {
        len++;
    }

    return len;
}

char *strcpy(char *dest, const char *src) 
{
    size_t i = 0;

    while ((dest[i] = src[i]))
    {
        i++;
    }

    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) 
{
    size_t i = 0;

    for (; i < n && src[i]; i++)
        dest[i] = src[i];

    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}

int strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }

    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char *s1, const char *s2, size_t n) 
{
    for (size_t i = 0; i < n; i++) 
    {
        if (s1[i] != s2[i] || !s1[i] || !s2[i])
        {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }

    return 0;
}

char *strchr(const char *s, int c) 
{
    while (*s) 
    {
        if (*s == (char)c)
        {
            return (char *)s;
        }

        s++;
    }

    return (c == 0) ? (char *)s : 0;
}

char *strrchr(const char *s, int c) 
{
    const char *last = 0;

    while (*s) 
    {
        if (*s == (char)c)
        {
            last = s;
        }

        s++;
    }

    if (c == 0)
    {
        return (char *)s;
    }

    return (char *)last;
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
    {
        return 0;
    }

    while (n-- && *s1 && *s2)
    {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);

        if (c1 != c2)
            return c1 - c2;

        s1++;
        s2++;
    }

    if (n == (size_t)-1)
    {
        return 0;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

int strcasecmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;

    while (*s1 && *s2)
    {
        c1 = (unsigned char)tolower((unsigned char)*s1);
        c2 = (unsigned char)tolower((unsigned char)*s2);

        if (c1 != c2)
        {
            return c1 - c2;
        }

        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

int remove(const char *path)
{
    (void)path;
    return -1;
}

int rename(const char *old, const char *new)
{
    (void)old;
    (void)new;
    return -1;
}

char *strdup(const char *s)
{
    size_t len = strlen(s);
    char *p = (char *)malloc(len + 1);

    if (!p)
    {
        return 0;
    }

    for (size_t i = 0; i <= len; i++)
    {
        p[i] = s[i];
    }

    return p;
}

const char *strstr(const char *haystack, const char *needle)
{
    if (!*needle)
    {
        return haystack;
    }

    for (; *haystack; haystack++)
    {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (*h == *n))
        {
            h++;
            n++;
        }

        if (!*n)
        {
            return haystack;
        }
    }

    return 0;
}
