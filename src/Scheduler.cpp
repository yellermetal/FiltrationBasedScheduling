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

double comp_time(lms_config_ptr config_list, double reconfiguration_delay) {

	int config_num = 0;
	int service_time = 0;
	lms_config_ptr iter = config_list->next;

	while (config_list != NULL) {

		service_time += config_list->coeff;
		config_num++;

		config_list = iter;
		if (iter != NULL)
			iter = iter->next;
	}
	return service_time + reconfiguration_delay * config_num;
}

Scheduler::Scheduler(string _name, int _switchRadix, double _reconfig_penalty, lms_config_ptr(*_scheduler)(lms_mat_ptr, double))
{
	switchRadix = _switchRadix;
	reconfig_penalty = _reconfig_penalty;
	config_queue = new ConfigQueue(switchRadix);
	scheduler = _scheduler;
	name = _name;

	schedule = nullptr;
	runtimeDelay = 0;
	schedulingDelay = TRIVIAL_DELAY;
	adaptiveSchedulingDelay = false;
}

Scheduler::~Scheduler()
{
	delete config_queue;
}

bool Scheduler::readyToSchedule()
{
	return (runtimeDelay == 0) && (schedulingDelay == 0);
}

void Scheduler::Schedule(int ** demandMatrix)
{
	assert(demandMatrix != nullptr);

	if (isDemandMatrixEmpty(demandMatrix, switchRadix)) {
		freeDemandMatrix(demandMatrix, switchRadix);
		return;
	}

	lms_mat_ptr mat_ptr = Make_Matrix(switchRadix, demandMatrix);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	schedule = scheduler(mat_ptr, reconfig_penalty);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double, micro> fp_us = t2 - t1;
	runtimeDelay = fp_us.count();

	if (adaptiveSchedulingDelay)
		schedulingDelay = runtimeDelay + max(switchRadix * reconfig_penalty, 
				comp_time(schedule,reconfig_penalty) + config_queue->getDemandCompletionTime(reconfig_penalty)
				- runtimeDelay);
	else
		schedulingDelay = max(TRIVIAL_DELAY, runtimeDelay);

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

void Scheduler::useAdaptiveSchedulingDelay(bool adaptive)
{
	adaptiveSchedulingDelay = adaptive;
}

string Scheduler::getName() {
	return name;
}

void Scheduler::update(int clock)
{

	if (runtimeDelay > 0) {
		runtimeDelay--;

		if (runtimeDelay == 0) {
			config_queue->enqueue(schedule);
			if (schedule)
				cout << name << " commited a schedule." << endl;
			schedule = nullptr;
		}
	}

	if (schedulingDelay > 0)
		schedulingDelay--;

	else
		cout << "Starting scheduling," << " time: " << clock << endl;

}
