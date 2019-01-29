
#ifndef CLOCK_H
#define CLOCK_H


#include "Observable.h"
class Clock :
	public Observable
{
public:
	Clock();
	int time();
	int tick();
	void reset();

private:
	int currTime;
};

#endif //CLOCK_H