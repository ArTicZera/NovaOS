#ifndef MEM_H
#define MEM_H

void* memset(void* dst, char val, DWORD num);
void* memcpy(void *dest, const void *src, DWORD n);
int strcmp(const char *str1, const char *str2);
char* strncpy(char *dest, const char *src, DWORD n);
int strncmp(const char *str1, const char *str2, DWORD n);
DWORD strlen(const char *str);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);
void* memmove(void* dest, const void* src, DWORD n);
char* strrchr(const char* str, int c);
int memcmp(const void *s1, const void *s2, DWORD n);

#endif
