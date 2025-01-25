/*
    Coded by ArTic/JhoPro

    This Memory Allocation system uses memory blocks.
    It's very simple and I like it
*/

#include "../Include/stdint.h"

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

void FreeMemory(void* ptr, DWORD size) 
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
