#ifndef _TIMER_H_
#define _TIMER_H_


/**> HEADER FILES <**/
class timer			 							
{
	public:
  	unsigned long long mm_timer_start;			
};


void TimerInit(timer* theTimer);
double TimerGetTime(timer* theTimer);

#endif

