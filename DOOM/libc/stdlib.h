#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

typedef unsigned long long uint64;

void *malloc(size_t nbytes);
void free(void *ap);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

int abs(int x);
double fabs(double x);

int atoi(const char *s);
double atof(const char *s);
uint64 __divdi3(uint64 num, uint64 den);

#endif
