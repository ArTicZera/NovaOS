#include "qjs_port.h"

VOID *JS_Def_MAlloc(JSMallocState *s, DWORD size)
{
    (void)s;
    return kmalloc(size);
}

VOID JS_Def_Free(JSMallocState *s, VOID *ptr)
{
    (void)s;
    kfree(ptr);
}

VOID *JS_Def_REalloc(JSMallocState *s, VOID *ptr, DWORD size)
{
    (void)s;
    kfree(ptr);
}