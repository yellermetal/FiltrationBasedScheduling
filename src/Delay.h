
#ifndef DELAY_H
#define DELAY_H

#include "Clock.h"
#include "ConfigQueue.h"
#include <assert.h>
#include <math.h>

#define TRIVIAL_DELAY 3000

class Delay:
	public Observer
{
public:
	Delay() :delay(0) {};
	Delay(int delay);
	virtual ~Delay() {};
	int getDelay();
	void setDelay(int runtime);
	void update(Observable& observable);
	
protected:
	int delay;
};

class AdaptiveDelay :
	public Delay
{
public:
	AdaptiveDelay(int switchRadix, int reconfig_penalty, ConfigQueue& configQueue);
	void setDelay(int runtime);

private:
	int lastRuntime;
	int switchRadix;
	int reconfig_penalty;
	ConfigQueue& configQueue;
};

#endif //DELAY_H