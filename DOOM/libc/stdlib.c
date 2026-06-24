/* From CrowOS */

#include "stdlib.h"
#include "string.h"
#include "unistd.h"

typedef long Align;

union header 
{
  struct 
  {
    union header *ptr;
    size_t size;
  } s;

  Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

void free(void *ap)
{
    if (!ap)
        return;

    Header *bp = (Header *)ap - 1;
    Header *p;

    if (freep == NULL) {
        base.s.ptr = &base;
        base.s.size = 0;
        freep = &base;
    }

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;
    }

    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }

    freep = p;
}

#define NALLOC 1024

static Header *morecore(size_t nu)
{
    char *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;

    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *)-1)
        return NULL;

    up = (Header *)cp;
    up->s.size = nu;
    
    free((void *)(up + 1));

    return freep;
}

void *malloc(size_t nbytes)
{
    Header *p, *prevp;
    size_t nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if (freep == NULL) {
        base.s.ptr = &base;
        base.s.size = 0;
        freep = &base;
    }

    prevp = freep;

    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            } else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freep = prevp;
            return (void *)(p + 1);
        }

        if (p == freep) {
            p = morecore(nunits);
            if (!p)
                return NULL;
        }
    }
}

void *calloc(size_t nmemb, size_t size)
{
    if (nmemb != 0 && size > 0xFFFFFFFF / nmemb)
        return NULL;

    size_t total = nmemb * size;

    void *data = malloc(total);
    if (!data)
        return NULL;

    memset(data, 0, total);
    return data;
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return malloc(size);

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    Header *h = (Header *)ptr - 1;

    size_t old_size = h->s.size * sizeof(Header);

    void *new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;

    size_t copy = old_size < size ? old_size : size;
    memcpy(new_ptr, ptr, copy);

    free(ptr);

    return new_ptr;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}

int atoi(const char *s)
{
    int sign = 1;
    int result = 0;

    // pular espaços
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
    {
        s++;
    }

    // sinal
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    // números
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }

    return result * sign;
}

double atof(const char *s)
{
    double result = 0.0;
    double frac = 0.0;
    double div = 1.0;
    int sign = 1;

    /* pula espaços */
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
        s++;

    /* sinal */
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    /* parte inteira */
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10.0 + (*s - '0');
        s++;
    }

    /* parte decimal */
    if (*s == '.')
    {
        s++;

        while (*s >= '0' && *s <= '9')
        {
            frac = frac * 10.0 + (*s - '0');
            div *= 10.0;
            s++;
        }

        result += frac / div;
    }

    return result * sign;
}

double fabs(double x)
{
  return (x < 0.0) ? -x : x;
}

uint64 __divdi3(uint64 num, uint64 den)
{
    uint64 res = 0;
    uint64 bit = 1;

    while (den <= num && (den << 1) > den)
    {
        den <<= 1;
        bit <<= 1;
    }

    while (bit)
    {
        if (num >= den)
        {
            num -= den;
            res |= bit;
        }

        den >>= 1;
        bit >>= 1;
    }

    return res;
}