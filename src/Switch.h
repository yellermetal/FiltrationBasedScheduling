
#ifndef SWITCH_H
#define SWITCH_H

#include "Observer.h"
#include "Clock.h"
#include "Config.h"
#include "Demand.h"
#include "Scheduler.h"


class Switch :
	public Observer
{
public:
	Switch(int switchRadix, Demand& demand, Scheduler& scheduler);
	void update(Observable& observable);


private:
	int switchRadix;
	Config* currConfig;
	Demand& demand;
	Scheduler& scheduler;


};

#endif //SWITCH_H