#include "Support.h"

#include <unistd.h>

#include "SDL.h"

void Microseconds(UnsignedWide *microTickCount)
{
	/* NOTE: hi isn't used in BS, so it's not implemented here */
	/* TODO: does game need microsecond precision? */
	microTickCount->hi = 0;
	microTickCount->lo = SDL_GetTicks() * 1000;
}

OSErr FSRead(int refNum, long *count, void *buffPtr)
{
	read(refNum, buffPtr, *count);
	
	return 0;
}

void GetMouse(Point *p)
{
STUB_FUNCTION;
}

void GetKeys(unsigned long *keys)
{
STUB_FUNCTION;
}

int Button(void)
{
STUB_FUNCTION;
	return 0;
}

void InitMouse()
{
STUB_FUNCTION;
}

void MoveMouse(int xcoord, int ycoord, Point *mouseloc)
{
STUB_FUNCTION;
}

void DisposeMouse()
{
STUB_FUNCTION;
}
