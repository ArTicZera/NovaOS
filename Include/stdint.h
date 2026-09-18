/*
  Coded by ArTic/JhoPro

  Just declares some short definitions for some variable.
*/

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef struct
{
    int64_t low;
    int64_t high;
} int128_t;

typedef unsigned char  uint8_t,  BYTE, UCHAR; // 8 bits, byte, unsigned char
typedef unsigned short uint16_t, WORD, DBYTE, UWORD, USHORT; // 16 bits, word, double-byte, unsigned word, unsigned short
typedef unsigned int   uint32_t, DWORD, QBYTE, UINT; // 32 bits, double-word, quad-byte, unsigned int
typedef unsigned long  ODWORD, ULONG; // 32 bits too, other double-word defintion, unsigned long
typedef unsigned int size_t; // size definition
typedef unsigned long long uint64_t, QWORD, OBYTE, UIntPtr, ULLONG; // 64 bits, quad-word, octa-byte, unsigned int pointer, unsigned long long

typedef struct
{
    uint64_t low;
    uint64_t high;
} uint128_t, OWORD, STBYTE, DQWORD; // 128 bits, octa-word, sixteen-byte, double-quad-word

typedef char   CHAR; // character
typedef short  SHORT; // short
typedef int    INT; // integer
typedef float  FLOAT; // floating integer
typedef double DOUBLE; // double integer
typedef long   LONG; // long

typedef long long IntPtr; // int pointer

#define CONST const // constant
#define VOID void // empty

typedef CONST VOID CVOID;

typedef BYTE*  LPBYTE;
typedef WORD*  LPWORD;
typedef DWORD* LPDWORD;
typedef QWORD* LPQWORD;
typedef ODWORD* LPODWORD;
typedef OWORD* LPOWORD;
typedef DBYTE* LPDBYTE;
typedef QBYTE* LPQBYTE;
typedef OBYTE* LPOBYTE;
typedef VOID* LPVOID;
typedef CVOID* LPCVOID;
typedef UCHAR* LPUCHAR;
typedef UWORD* LPUWORD;
typedef USHORT* LPUSHORT;
typedef UINT* LPUINT;
typedef ULONG* LPULONG;
typedef ULLONG* LPULLONG;

typedef const char* STRING;
typedef LPVOID HANDLE;

#define NULL  ((void*)0)

#define TRUE  1
#define true  1

#define FALSE 0
#define false 0

#define in
#define out
