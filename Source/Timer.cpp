/**> HEADER FILES <**/
#include <SDL3/SDL.h>
#include "Timer.h"				


/********************> Timer <*****/
void TimerInit(timer* theTimer)
{
	theTimer->mm_timer_start = SDL_GetTicksNS();
}

double TimerGetTime(timer* theTimer)
{
	return (double)(SDL_GetTicksNS() - theTimer->mm_timer_start);
}
