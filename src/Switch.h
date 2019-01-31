
#ifndef SWITCH_H
#define SWITCH_H

#include "Config.h"
#include "Demand.h"
#include "Scheduler.h"

class Switch
{
public:
	Switch(int switchRadix, Demand& demand, Scheduler* scheduler);
	void update(int clock);


private:
	int switchRadix;
	Config* currConfig;
	Demand& demand;
	Scheduler* scheduler;


};

#endif //SWITCH_H
