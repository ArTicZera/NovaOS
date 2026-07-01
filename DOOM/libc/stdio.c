//DISABLED DEBUG

#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "unistd.h"

FILE _stdin  = { .fd = 0 };
FILE _stdout = { .fd = 1 };
FILE _stderr = { .fd = 2 };

FILE *stdin  = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

static void fputc_internal(FILE *f, char c) 
{
    //write(f->fd, &c, 1);
}

static void fputs_internal(FILE *f, const char *s) 
{
    //write(f->fd, s, strlen(s));
}

void putchar(char c) 
{ 
    //write(stdout->fd, &c, 1); 
}

int printf(const char *fmt, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, fmt);

    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);

    //write(1, buffer, len);

    return len;
}

static void print_int(FILE *f, int n) 
{
    char buf[16];
    int i = 0;

    if (n == 0) 
    {
        fputc_internal(f, '0');
        return;
    }

    if (n < 0) 
    {
        fputc_internal(f, '-');
        n = -n;
    }

    while (n > 0) 
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i--)
    {
        fputc_internal(f, buf[i]);
    }
}

/* imprime hexadecimal */
static void print_hex(FILE *f, unsigned int n) 
{
    char buf[16];
    int i = 0;

    if (n == 0) 
    {
        fputc_internal(f, '0');
        return;
    }

    while (n > 0) 
    {
        int digit = n & 0xF;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        n >>= 4;
    }

    while (i--)
    {
        fputc_internal(f, buf[i]);
    }
}

static void putc_buf(char **buf, size_t *n, char c) 
{
    if (*n > 1) 
    {
        **buf = c;
        (*buf)++;
        (*n)--;
    }
}

static void puts_buf(char **buf, size_t *n, const char *s) 
{
    while (*s) 
    {
        putc_buf(buf, n, *s++);
    }
}

static void putint_buf(char **buf, size_t *n, int v) 
{
    char tmp[16];
    int i = 0;

    if (v == 0) 
    {
        putc_buf(buf, n, '0');
        return;
    }

    if (v < 0) 
    {
        putc_buf(buf, n, '-');
        v = -v;
    }

    while (v > 0) 
    {
        tmp[i++] = '0' + (v % 10);
        v /= 10;
    }

    while (i--)
    {
        putc_buf(buf, n, tmp[i]);
    }
}

static void skip_spaces(const char **s)
{
    while (**s == ' ' || **s == '\t' || **s == '\n')
        (*s)++;
}

int fprintf(FILE *f, const char *fmt, ...) 
{
    va_list args;
    va_start(args, fmt);

    int count = 0;

    for (int i = 0; fmt[i]; i++) 
    {
        if (fmt[i] == '%') 
        {
            i++;

            switch (fmt[i]) 
            {
                case 'd': 
                {
                    int n = va_arg(args, int);
                    print_int(f, n);
                    break;
                }

                case 'x': 
                {
                    unsigned int n = va_arg(args, unsigned int);
                    print_hex(f, n);
                    break;
                }

                case 's': 
                {
                    char *s = va_arg(args, char*);
                    fputs_internal(f, s);
                    break;
                }

                case 'c': 
                {
                    char c = (char)va_arg(args, int);
                    fputc_internal(f, c);
                    break;
                }

                case '%': 
                {
                    fputc_internal(f, '%');
                    break;
                }

                default:
                    break;
            }

        } 
        else 
        {
            fputc_internal(f, fmt[i]);
        }

        count++;
    }

    va_end(args);
    return count;
}

FILE *fopen(const char *path, const char *mode) 
{
    int fd = open(path, 0);

    if (fd < 0) 
    {
        return 0;
    }

    FILE *f = malloc(sizeof(FILE));
    f->fd = fd;
    f->pos = 0;
    f->size = 0;
    f->flags = 0;
    f->buffer = 0;

    return f;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f) 
{
    int total = size * nmemb;
    int bytes = read(f->fd, ptr, total);

    if (bytes < 0) 
    {
        return 0;
    }

    f->pos += bytes;
    return bytes / size;
}

int fclose(FILE *f) 
{
    close(f->fd);

    free(f);
    return 0;
}

int snprintf(char *buf, size_t n, const char *fmt, ...) 
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vsnprintf(buf, n, fmt, ap);
    va_end(ap);

    return ret;
}

long fseek(FILE *f, long offset, int whence)
{
    if (!f)
    {
        return -1;
    }

    long new_pos;

    switch (whence)
    {
        case SEEK_SET:
            new_pos = offset;
            break;

        case SEEK_CUR:
            new_pos = f->pos + offset;
            break;

        case SEEK_END:
            new_pos = lseek(f->fd, 0, SEEK_END) + offset;
            break;

        default:
            return -1;
    }

    /* sincroniza com kernel */
    long res = lseek(f->fd, new_pos, SEEK_SET);

    if (res < 0)
    {
        return -1;
    }

    f->pos = new_pos;
    return 0;
}

long ftell(FILE *f)
{
    return f ? f->pos : -1;
}

int vsnprintf(char *buf, size_t n, const char *fmt, va_list args)
{
    char *start = buf;
    size_t left = n;

    if (n == 0)
        return 0;

    for (int i = 0; fmt[i] && left > 1; i++)
    {
        if (fmt[i] == '%')
        {
            i++;

            if (!fmt[i])
                break;

            switch (fmt[i])
            {
                case 'd':
                {
                    int v = va_arg(args, int);

                    // simples int -> string (suficiente pro Doom)
                    char tmp[32];
                    int t = 0;

                    if (v == 0)
                    {
                        *buf++ = '0';
                        left--;
                        break;
                    }

                    if (v < 0)
                    {
                        *buf++ = '-';
                        left--;
                        v = -v;
                    }

                    while (v > 0 && t < 32)
                    {
                        tmp[t++] = '0' + (v % 10);
                        v /= 10;
                    }

                    while (t-- && left > 1)
                    {
                        *buf++ = tmp[t];
                        left--;
                    }
                    break;
                }

                case 's':
                {
                    char *s = va_arg(args, char*);
                    while (*s && left > 1)
                    {
                        *buf++ = *s++;
                        left--;
                    }
                    break;
                }

                case 'c':
                {
                    char c = (char)va_arg(args, int);
                    *buf++ = c;
                    left--;
                    break;
                }

                case '%':
                    *buf++ = '%';
                    left--;
                    break;

                default:
                    *buf++ = '%';
                    *buf++ = fmt[i];
                    left -= 2;
                    break;
            }
        }
        else
        {
            *buf++ = fmt[i];
            left--;
        }
    }

    *buf = '\0';

    return (int)(buf - start);
}

int vfprintf(FILE *f, const char *fmt, va_list args)
{
    char buffer[1024];

    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (len < 0)
    {
        return -1;
    }

    write(f->fd, buffer, len);

    return len;
}

int fflush(FILE *f)
{
    (void)f;
    return 0;
}

int puts(const char *s)
{
    if (!s)
    {
        return -1;
    }

    fputs_internal(stdout, s);
    fputc_internal(stdout, '\n');

    return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f)
{
    if (!ptr || !f || size == 0 || nmemb == 0)
    {
        return 0;
    }

    size_t total = size * nmemb;

    int written = write(f->fd, ptr, total);

    if (written < 0)
    {
        return 0;
    }

    f->pos += written;

    return written / size;
}

int sscanf(const char *str, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int count = 0;

    while (*fmt && *str)
    {
        if (*fmt == '%')
        {
            fmt++;

            switch (*fmt)
            {
                case 'd':
                {
                    int *out = va_arg(args, int*);
                    int sign = 1;
                    int val = 0;

                    skip_spaces(&str);

                    if (*str == '-')
                    {
                        sign = -1;
                        str++;
                    }
                    else if (*str == '+')
                    {
                        str++;
                    }

                    while (*str >= '0' && *str <= '9')
                    {
                        val = val * 10 + (*str - '0');
                        str++;
                    }

                    *out = val * sign;
                    count++;
                    break;
                }

                case 's':
                {
                    char *out = va_arg(args, char*);

                    skip_spaces(&str);

                    while (*str && *str != ' ' && *str != '\t' && *str != '\n')
                    {
                        *out++ = *str++;
                    }

                    *out = '\0';
                    count++;
                    break;
                }

                case 'c':
                {
                    char *out = va_arg(args, char*);
                    *out = *str++;
                    count++;
                    break;
                }

                default:
                    break;
            }

            fmt++;
        }
        else
        {
            if (*fmt != *str)
            {
                break;
            }

            fmt++;
            str++;
        }
    }

    va_end(args);
    return count;
}    
