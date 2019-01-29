
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ConfigQueue.h"
#include "Delay.h"
#include "Algorithms.h"
#include <assert.h>
#include <chrono>
#include <string>
using namespace std::chrono;

class Scheduler:
	public Observer
{
public:
	Scheduler(string name, int switchRadix, double reconfig_penalty, 
		Clock& clock, lms_config_ptr(*scheduler)(lms_mat_ptr, double));
	virtual ~Scheduler();

	bool readyToSchedule();
	void Schedule(int** demandMatrix);
	void useAdaptiveSchedulingDelay(Clock& clock, bool chosen);
	Config* getNextConfig();
	
	FILE* openFile();
	void update(Observable& observable);
	

private:
	
	ConfigQueue* config_queue;
	Delay* schedulingDelay;
	Delay* runtimeDelay;
	int switchRadix;
	double reconfig_penalty;
	int runtime;
	lms_config_ptr(*scheduler)(lms_mat_ptr, double);
	lms_config_ptr schedule;
	string name;


};

class Lumos :
	public Scheduler
{
public:
	Lumos(int switchRadix, double reconfig_penalty,	Clock& clock):
		Scheduler("Lumos", switchRadix, reconfig_penalty, clock, LUMOS_Decomp) {};
};

class Solstice :
	public Scheduler
{
public:
	Solstice(int switchRadix, double reconfig_penalty, Clock& clock) :
		Scheduler("Solstice", switchRadix, reconfig_penalty, clock, SOLSTICE_Decomp) {};
};

class Eclipse :
	public Scheduler
{
public:
	Eclipse(int switchRadix, double reconfig_penalty, Clock& clock) :
		Scheduler("Eclipse", switchRadix, reconfig_penalty, clock, ECLIPSE_Decomp) {};
};

#endif //SCHEDULER_H