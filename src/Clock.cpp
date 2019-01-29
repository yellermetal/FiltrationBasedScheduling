#include "Clock.h"



Clock::Clock()
{
	currTime = 0;
}

int Clock::time()
{
	return currTime;
}

int Clock::tick()
{
	currTime++;
	notifyAllObservers();
}

void Clock::reset()
{
	currTime = 0;
}
