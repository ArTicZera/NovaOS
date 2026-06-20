#ifndef MEM_H
#define MEM_H

void* memset(void* dst, char val, unsigned int num);
void* memcpy(void *dest, const void *src, unsigned int n);
int strcmp(const char *str1, const char *str2);
char* strncpy(char *dest, const char *src, unsigned int n);
int strncmp(const char *str1, const char *str2, unsigned int n);
unsigned int strlen(const char *str);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);
void* memmove(void* dest, const void* src, unsigned int n);
char* strrchr(const char* str, int c);
int memcmp(const void *s1, const void *s2, unsigned int n);
int strncasecmp(const char *s1, const char *s2, unsigned int n);
int strcasecmp(const char *s1, const char *s2);
char *strchr(const char *s, int c);
char *strdup(const char *s);

#endif