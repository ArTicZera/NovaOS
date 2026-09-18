// QuickJS is a engine that run Javascript files, i think

#include "../include/WebArchy.h"
#include "qjs_include.h"

// To use this functions you can switch for your
// data from memory manipulation functions
extern LPVOID kmalloc(DWORD size);
extern VOID kfree(VOID* ptr);
extern LPVOID krealloc(LPVOID ptr, DWORD size);

VOID *JS_Def_MAlloc(JSMallocState *s, DWORD size);
VOID JS_Def_Free(JSMallocState *s, VOID *ptr);
VOID *JS_Def_REalloc(JSMallocState *s, VOID *ptr, DWORD size);

const JSMallocFunctions WebArchy_JS_Memory_Alloc_Functions =
{
    JS_Def_MAlloc, JS_Def_Free, JS_Def_REalloc,
    NULL // MAlloc_Usable_Size (optional)
};