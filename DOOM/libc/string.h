#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdarg.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
const char *strstr(const char *haystack, const char *needle);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

int strncasecmp(const char *s1, const char *s2, size_t n);
int strcasecmp(const char *s1, const char *s2);

int remove(const char *path);
int rename(const char *old, const char *new);

char *strdup(const char *s);
int atoi(const char *s);

int abs(int x);

#endif
