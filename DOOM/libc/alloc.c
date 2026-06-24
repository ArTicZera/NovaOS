/*
    Coded by ArTic/JhoPro

    This Memory Allocation system uses memory blocks.
    It's very simple and I like it
*/

#include "../../Include/stdint.h"

#include "../../Memory/mem.h"
#include "alloc.h"

MemoryManager mgr;

void InitMemory() 
{
    mgr.freeBlocks = (MemoryBlock*)mgr.memory;
    mgr.freeBlocks->next = NULL;
    mgr.freeBlocks->size = MEMPOOL - BLOCK_SIZE;
    mgr.freeBlocks->used = 0;
}

void* AllocateMemory(DWORD size) 
{
    MemoryBlock* block = mgr.freeBlocks;
    MemoryBlock* prev = NULL;

    size = (size + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;

    while (block) 
    {
        if (!block->used && block->size >= size) 
        {
            if (block->size > size + BLOCK_SIZE) 
            {
                MemoryBlock* newBlock = (MemoryBlock*)((BYTE*)block + BLOCK_SIZE + size);
                newBlock->size = block->size - size - BLOCK_SIZE;
                newBlock->used = 0;
                newBlock->next = block->next;

                block->next = newBlock;
                block->size = size;
            }

            block->used = 1;
            return (BYTE*)block + BLOCK_SIZE;
        }

        prev = block;
        block = block->next;
    }

    return NULL;
}

void FreeMemory(void* ptr) 
{
    if (!ptr) return;

    MemoryBlock* block = (MemoryBlock*)((BYTE*)ptr - BLOCK_SIZE);
    block->used = 0;

    MemoryBlock* current = mgr.freeBlocks;
    
    while (current) 
    {
        if ((BYTE*)current + BLOCK_SIZE + current->size == (BYTE*)block) 
        {
            current->size += BLOCK_SIZE + block->size;
            current->next = block->next;
            
            return;
        }

        if ((BYTE*)block + BLOCK_SIZE + block->size == (BYTE*)current)
        {
            block->size += BLOCK_SIZE + current->size;
            block->next = current->next;

            if (mgr.freeBlocks == current) 
            {
                mgr.freeBlocks = block;
            }
            
            return;
        }

        current = current->next;
    }

    block->next = mgr.freeBlocks;
    mgr.freeBlocks = block;
}

void* CAllocateMemory(int count, int size)
{
    int total = count * size;

    void* ptr = AllocateMemory(total);
    
    if (!ptr)
    {
        return NULL;
    }

    memset(ptr, 0x00, total);
    
    return ptr;
}

void* ReAllocateMemory(void* ptr, DWORD newSize)
{
    if(ptr == NULL)
    {
        return AllocateMemory(newSize);
    }

    if(newSize == 0)
    {
        FreeMemory(ptr);
        return NULL;
    }

    MemoryBlock* block = (MemoryBlock*)((BYTE*)ptr - BLOCK_SIZE);

    #define ALIGN 4

    newSize = (newSize + ALIGN - 1) & ~(ALIGN - 1);

    if(block->size >= newSize)
    {
        return ptr;
    }

    MemoryBlock* next = block->next;

    if(next && !next->used && (block->size + BLOCK_SIZE + next->size) >= newSize)
    {
        block->size += BLOCK_SIZE + next->size;
        block->next = next->next;

        return ptr;
    }

    void* newPtr = AllocateMemory(newSize);

    if(newPtr == NULL)
    {
        return NULL;
    }

    memcpy(newPtr, ptr, block->size);

    FreeMemory(ptr);

    return newPtr;
}