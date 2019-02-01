
#ifndef SWITCH_H
#define SWITCH_H

#include "Config.h"
#include "Demand.h"
#include "Scheduler.h"

class Switch
{
public:
	Switch(int switchRadix, int reconfig_penalty, Demand& demand, Scheduler* scheduler);
	void update(int clock);


private:
	int switchRadix;
	int reconfig_penalty;
	int reconfig_delay;
	Config* currConfig;
	Demand& demand;
	Scheduler* scheduler;


};

#endif //SWITCH_H
