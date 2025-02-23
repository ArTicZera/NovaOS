#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Drivers/sb16.h"

void _start()
{
    BYTE test[64000];

    for (int t = 0; t < 64000; t++)
    {
        test[t] = t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & -t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2);
    }

    PlayBytebeat(test, sizeof(test));
}