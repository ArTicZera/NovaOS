#include <stdio.h>
#include <alloc.h>

#include "m_argv.h"

#include "doomgeneric.h"

pixel_t* DG_ScreenBuffer = NULL;

void M_FindResponseFile(void);
void D_DoomMain (void);


void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	M_FindResponseFile();

	DG_ScreenBuffer = AllocateMemory(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

	if (!DG_ScreenBuffer) printf("Failed Allocating Memory");

	DG_Init();

	D_DoomMain ();
}

