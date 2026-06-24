#ifndef _ALLOC_H
#define _ALLOC_H

#include "../../Include/stdint.h"

#define MEMPOOL 0xF00000 // Exemplo: 1 MB de memória para alocação
#define BLOCK_SIZE sizeof(MemoryBlock)

typedef struct MemoryBlock {
    struct MemoryBlock* next;
    DWORD size;
    BYTE used;
} MemoryBlock;

typedef struct {
    MemoryBlock* freeBlocks;
    BYTE memory[MEMPOOL];
} MemoryManager;


void InitMemory();
void* AllocateMemory(DWORD size);
void FreeMemory(void* ptr);
void* CAllocateMemory(int count, int size);
void* ReAllocateMemory(void* ptr, DWORD newSize);

#endif