#include "Switch.h"



Switch::Switch(int _switchRadix, int _reconfig_penalty, Demand& _demand, Scheduler* _scheduler):
	switchRadix(_switchRadix), reconfig_penalty(_reconfig_penalty), reconfig_delay(_reconfig_penalty),
	currConfig(nullptr), demand(_demand), scheduler(_scheduler) {}

void Switch::update(int clock)
{

	if (scheduler->readyToSchedule()) {
		cout << "Switch: " << "Begin Scheduling, time: " << clock << endl;
		scheduler->Schedule(demand.getDemand());
	}

	if (reconfig_delay > 0)
		reconfig_delay--;

	if (!currConfig && reconfig_delay == 0)
		currConfig = scheduler->getNextConfig();

	if (currConfig && currConfig->getTimeDuration() > 0) {
		demand.serveDemand(currConfig);
		currConfig->decrementTimeDuration();
		
		if (currConfig->getTimeDuration() == 0) {
			delete currConfig;
			currConfig = nullptr;
			reconfig_delay = reconfig_penalty;
		}
	}

}
