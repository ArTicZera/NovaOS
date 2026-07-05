// doomgeneric for NovaOS

#include "doomkeys.h"
#include "i_system.h"
#include "i_video.h"
#include "m_argv.h"
#include "doomgeneric.h"
#include "doom_input.h"

#include "../Include/stdint.h"
#include "../Include/ports.h"
#include "../Interrupts/idt.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <alloc.h>
#include <string.h>
#include <ctype.h>

static unsigned char keyDown[128] = {0};
static unsigned short s_KeyQueue[128];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;
static unsigned int* temp_bitmap = NULL;

void DG_Init() 
{
	temp_bitmap = (unsigned int*) 0xFD000000;
}

void DG_DrawFrame()
{
    for (int y = 0; y < DOOMGENERIC_RESY; y++)
    {
        for (int x = 0; x < DOOMGENERIC_RESX; x++)
        {
            int scrX = x + 320;
            int scrY = y + 180;

            temp_bitmap[scrY * 1280 + scrX] = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
        }
    }
}

void addKeyToQueue(int pressed, unsigned int keyCode)
{
	unsigned char key = convertToDoomKey(keyCode);

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= 128;
}

unsigned char convertToDoomKey(unsigned int key)
{
	switch(key)
	{
		case 0x11: // W
		case 0x48:
			return KEY_UPARROW;

		case 0x1F:
		case 0x50: // S
			return KEY_DOWNARROW;

		case 0x1E:
		case 0x4B: // A
			return KEY_LEFTARROW;

		case 0x20: // D
		case 0x4D:
			return KEY_RIGHTARROW;

		case 0x39: // Space
			return KEY_USE;

		case 0x1C: // Enter
			return KEY_ENTER;

		case 0x1D: //co
			return KEY_FIRE;

		case 0x01: // Esc
			return KEY_ESCAPE;
	}

	return key;
}


void DG_SleepMs(uint32_t ms)
{
	for (volatile int i = 0; i < ms * 100000; i++)
	{

	}
}


uint32_t DG_GetTicksMs()
{
    static uint32_t t = 0;

    t += 1;

    return t;
}


void DG_SetWindowTitle(const char *title)
{
	
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
	{
		return 0;
	}
	else
	{
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= 128;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

int _start(int argc, char** argv)
{
	//char* argv[] = { "doomgeneric", "-iwad", "doom1.wad", 0 };
	InitMemory();

	doomgeneric_Create(argc, argv);

	while (true)
	{
		doomgeneric_Tick();
		//Read scan code
		unsigned char scan = inb(0x60);

		//Check if its pressed
		unsigned char isPress = !(scan & 0x80);
		scan &= 0x7F;

		addKeyToQueue(isPress, scan);
		//I_StartFrame ();
	}

	return 0;
}
