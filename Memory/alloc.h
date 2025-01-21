#define MEMPOOL 0x100000 // Exemplo: 1 MB de memória para alocação
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
void FreeMemory(void* ptr, DWORD size);
