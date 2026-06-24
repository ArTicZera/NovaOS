#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>
#include <stdarg.h>

typedef struct 
{
    int fd;
    int pos;
    int size;
    int flags;
    void *buffer;
} FILE;;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

void putchar(char c);
int printf(const char *fmt, ...);

long ftell(FILE *f);
int fprintf(FILE *f, const char *fmt, ...);
FILE *fopen(const char *path, const char *mode);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f);
long fseek(FILE *f, long offset, int whence);
int fclose(FILE *f);
int fflush(FILE *f);

int snprintf(char *buf, size_t n, const char *fmt, ...);
int vsnprintf(char *buf, size_t n, const char *fmt, va_list args);
int vfprintf(FILE *f, const char *fmt, va_list args);
int sscanf(const char *str, const char *fmt, ...);

int puts(const char *s);

#endif
