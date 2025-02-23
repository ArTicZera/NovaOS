#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Font/text.h"

#include "sb16.h"

int playing;

void ResetSoundBlaster()
{
    outb(0x226, 0x01);
    outb(0x226, 0x00);
}

void WriteDSP(BYTE cmd)
{
    while ((inb(0x22C) & 0x80) != 0x00);
    outb(0x22C, cmd);
}

void SoundBlasterHandler()
{
    inb(0x22E);
    outb(0x20, 0x20);
    playing = 0x00;
}

void SoundBlasterInit()
{
    WriteDSP(0xD1);
}

void SoundBlasterDisable()
{
    WriteDSP(0xD3);
}

void SetupDMA(LPBYTE buffer, WORD size)
{
    uint64_t addr = (ULONG)buffer;

    outb(0x0A, 0x05);
    outb(0x0C, 0x00);
    outb(0x0B, 0x49);
    outb(0x02, addr & 0xFF);
    outb(0x02, (addr >> 8) & 0xFF);
    outb(0x83, (addr >> 16) & 0xFF);
    outb(0x03, (size - 1) & 0xFF);
    outb(0x03, (size - 1) >> 8);
    outb(0x0A, 0x01);
}

void SoundBlasterPlay(LPBYTE buffer, WORD size)
{
    playing = 0x01;

    // Configurar DMA antes de iniciar a reprodução
    SetupDMA(buffer, size);

    WriteDSP(0x40);
    WriteDSP(256 - (1000000 / 8000)); // 11 kHz

    WriteDSP(0x14);
    WriteDSP((size - 1) & 0xFF);
    WriteDSP((size - 1) >> 8);

    for (int i = 0; i < 10000000; i++);

    playing = 0x00;
}

void SetupSoundBlaster()
{
    ResetSoundBlaster();
    SoundBlasterInit();
}

void PlayBytebeat(LPBYTE buffer, WORD size)
{
    SoundBlasterPlay(buffer, size);

    while (playing);
    SoundBlasterDisable();
}