// doomgeneric for Allegro library

#include "doomkeys.h"
#include "i_system.h"
#include "i_video.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <alloc.h>
#include <string.h>

static bool videomode_set = false;
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
			temp_bitmap[y * 1280 + x] = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
		}
	}
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


int DG_GetKey(int *pressed, unsigned char *doomKey)
{
	return 0;
}


void DG_SetWindowTitle(const char *title)
{
	printf("DOOM");
}


int _start(int argc, char** argv)
{
	//char* argv[] = { "doomgeneric", "-iwad", "doom1.wad", 0 };
	InitMemory();

	doomgeneric_Create(argc, argv);

	while (true)
	{
		doomgeneric_Tick();

		extern unsigned int DG_GetTickMs();
	}

	return 0;
}
