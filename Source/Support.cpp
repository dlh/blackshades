#include "Support.h"

#include <unistd.h>

#include "SDL.h"

void Microseconds (UnsignedWide *microTickCount)
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
