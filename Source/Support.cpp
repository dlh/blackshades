#include "Support.h"

#include "SDL.h"

void Microseconds (UnsignedWide *microTickCount)
{
	/* NOTE: hi isn't used in BS, so it's not implemented here */
	/* TODO: does game need microsecond precision? */
	microTickCount->lo = SDL_GetTicks() * 1000;
}
