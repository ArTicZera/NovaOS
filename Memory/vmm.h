#define PAGE_SIZE 4096
#define TOTAL_PAGES 1024
#define VIRTUAL_MEMORY_SIZE (PAGE_SIZE * TOTAL_PAGES)

typedef struct PageTableEntry 
{
    DWORD physicalAddress;
    BYTE present;
    BYTE writable;
    BYTE user;
} PageTableEntry;

typedef struct VirtualMemoryManager 
{
    PageTableEntry pageTable[TOTAL_PAGES];
    BYTE physicalMemory[TOTAL_PAGES * PAGE_SIZE];
} VirtualMemoryManager;

void InitVirtualMemory();
int MapPage(DWORD virtualAddress, DWORD physicalAddress, BYTE writable, BYTE user);
void UnmapPage(DWORD virtualAddress);
DWORD TranslateAddress(DWORD virtualAddress);
void* AllocateVirtualMemory(DWORD virtualAddress, DWORD size, BYTE writable, BYTE user);
void FreeVirtualMemory(DWORD virtualAddress, DWORD size);