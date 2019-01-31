
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ConfigQueue.h"
#include "Algorithms.h"
#include <assert.h>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>

#define TRIVIAL_DELAY 3000

using namespace std::chrono;
using namespace std;

class Scheduler
{
public:
	Scheduler(string name, int switchRadix, double reconfig_penalty, 
		lms_config_ptr(*scheduler)(lms_mat_ptr, double));
	virtual ~Scheduler();

	bool readyToSchedule();
	void Schedule(int** demandMatrix);
	void useAdaptiveSchedulingDelay(bool chosen);
	Config* getNextConfig();
	
	void reset();
	void openFile(ofstream& file);
	void update(int clock);
	

private:
	
	ConfigQueue* config_queue;
	int switchRadix;
	double reconfig_penalty;
	int runtimeDelay;
	int schedulingDelay;
	bool adaptiveSchedulingDelay;
	lms_config_ptr(*scheduler)(lms_mat_ptr, double);
	lms_config_ptr schedule;
	string name;

};

/* ------------------------- Concrete Schedulers ------------------------- */

class Lumos :
	public Scheduler
{
public:
	Lumos(int switchRadix, double reconfig_penalty):
		Scheduler("Lumos", switchRadix, reconfig_penalty, LUMOS_Decomp) {};
};

class Solstice :
	public Scheduler
{
public:
	Solstice(int switchRadix, double reconfig_penalty) :
		Scheduler("Solstice", switchRadix, reconfig_penalty, SOLSTICE_Decomp) {};
};

class Eclipse :
	public Scheduler
{
public:
	Eclipse(int switchRadix, double reconfig_penalty) :
		Scheduler("Eclipse", switchRadix, reconfig_penalty, ECLIPSE_Decomp) {};
};

#endif //SCHEDULER_H
