
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

#define S2US 1000000
#define NS2US 1000

class Scheduler
{
public:

	/**
	 * Scheduler constructor
	 *
	 * @param: name : string - name of the scheduler
	 * @param: switchRadix : int - the size of the switch
	 * @param: reconfig_penalty : int - the switch reconfiguration penalty (~20us nominal)
	 * @param: scheduler : scheduling_function - C implemented scheduling algorithm
	 *
	 * @effects: constructs the C++ wrapper for the scheduling algorithm
	 */
	Scheduler(string name, int switchRadix, double reconfig_penalty, 
		lms_config_ptr(*scheduler)(lms_mat_ptr, double));

	/**
	 * Virtual Destructor - overriden by concrete scheduler classes
	 */
	virtual ~Scheduler();

	/**
	 * Checks scheduler state and returns whether
	 * scheduler is ready for a new scheduling procedure.
	 *
	 * @return: true if this scheduler is ready, false otherwise.
	 */
	bool readyToSchedule();

	/**
	 * Executes scheduling of demandMatrix
	 *
	 * @param: a non-null pointer to 2D-array of non-negative integers
	 *
	 * @effects: calculates a schedule for the demand matrix and saves it internally.
	 * 			 Does nothing if the demand matrix is empty ( zeros array )
	 */
	void Schedule(int** demandMatrix);

	/**
	 * Toggles the use of an adaptive scheduling delay.
	 * The adaptive delay is a function of scheduler runtime,
	 * as well as the overall demand completion time of all configurations in ConfigQueue.
	 *
	 * @param: adaptive : bool - client choice
	 * @effects: if 'adaptive' is true, uses the adaptive scheduling delay method,
	 * 			 else sets scheduling delay to be TRIVIAL_DELAY (~3ms nominal)
	 */
	void useAdaptiveSchedulingDelay(bool adaptive);

	/**
	 * Gets the next available configuration
	 *
	 * @return: the head of the ConfigQueue
	 */
	Config* getNextConfig();

	string getName();

	/**
	 *
	 */
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
