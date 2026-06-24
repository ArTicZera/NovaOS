#ifndef _FCNTL_H
#define _FCNTL_H

/* modos de abertura (flags para open) */
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2

/* flags extras comuns (Doom pode ignorar a maioria) */
#define O_CREAT  0x40
#define O_TRUNC  0x200
#define O_APPEND 0x400

#endif
