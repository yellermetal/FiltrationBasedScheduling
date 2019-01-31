#include "Switch.h"



Switch::Switch(int _switchRadix, Demand& _demand, Scheduler* _scheduler):
	switchRadix(_switchRadix), currConfig(nullptr), demand(_demand), scheduler(_scheduler) {}

void Switch::update(int clock)
{

	if (scheduler->readyToSchedule()) {
		cout << "Switch: " << "Begin Scheduling, time: " << clock << endl;
		scheduler->Schedule(demand.getDemand());
	}

	if (!currConfig)
		currConfig = scheduler->getNextConfig();

	if (currConfig && currConfig->getTimeDuration() > 0) {
		demand.serveDemand(currConfig);
		currConfig->decrementTimeDuration();
		
		if (currConfig->getTimeDuration() == 0) {
			delete currConfig;
			currConfig = nullptr;
		}
	}

}
