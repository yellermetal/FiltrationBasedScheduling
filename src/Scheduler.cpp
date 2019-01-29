#include "Scheduler.h"

bool isDemandMatrixEmpty(int** demandMatrix, int switchRadix) {
	for (int i = 0; i < switchRadix; i++)
		for (int j = 0; j < switchRadix; j++)
			if (demandMatrix[i][j] != 0)
				return false;
	return true;
}

void freeDemandMatrix(int** demandMatrix, int switchRadix) {
	for (int i = 0; i < switchRadix; i++)
		delete[] demandMatrix[i];
	delete[] demandMatrix;
}

Scheduler::Scheduler(string _name, int _switchRadix, double _reconfig_penalty, Clock & clock, lms_config_ptr(*_scheduler)(lms_mat_ptr, double))
	: config_queue(new ConfigQueue(_switchRadix)), schedulingDelay(new Delay()), runtimeDelay(new Delay()),
	  switchRadix(_switchRadix), reconfig_penalty(_reconfig_penalty), runtime(0), scheduler(_scheduler), schedule(nullptr), name(_name)
{
	clock.attach(*runtimeDelay);
	clock.attach(*schedulingDelay);
}

Scheduler::~Scheduler() {
	delete config_queue;
	delete runtimeDelay;
	delete schedulingDelay;
}

bool Scheduler::readyToSchedule()
{
	return (runtimeDelay->getDelay() == 0) && (schedulingDelay->getDelay() == 0);
}

void Scheduler::Schedule(int ** demandMatrix)
{
	assert(demandMatrix != nullptr);

	if (isDemandMatrixEmpty(demandMatrix, switchRadix))
		freeDemandMatrix(demandMatrix, switchRadix);

		lms_mat_ptr mat_ptr = Make_Matrix(switchRadix, demandMatrix);

		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		schedule = scheduler(mat_ptr, reconfig_penalty);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double, micro> fp_us = t2 - t1;
		delete runtimeDelay;
		runtimeDelay = new Delay(fp_us.count());
		runtime = runtimeDelay->getDelay();

		Free_Matrix(mat_ptr);
		freeDemandMatrix(demandMatrix, switchRadix);
}

Config * Scheduler::getNextConfig()
{
	if (config_queue->isEmpty())
		return nullptr;
	else
		return config_queue->dequeue();
}

void Scheduler::useAdaptiveSchedulingDelay(Clock& clock, bool chosen)
{
	delete schedulingDelay;
	if (chosen)
		schedulingDelay = new AdaptiveDelay(switchRadix, reconfig_penalty, *config_queue);
	else
		schedulingDelay = new Delay();

	clock.attach(*schedulingDelay);
}

FILE * Scheduler::openFile()
{
	string prefix = "./Results/";
	string suffix = ".txt";
	string filename = prefix + name + suffix;
	return fopen(filename.c_str(), "w");
}

void Scheduler::update(Observable& observable)
{
	if (runtimeDelay->getDelay() == 0) {
		config_queue->enqueue(schedule);
		schedule = nullptr;

		if (schedulingDelay->getDelay() == 0) {
			schedulingDelay->setDelay(runtime);
		}
	}
}
