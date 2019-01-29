#include "ConfigQueue.h"



ConfigQueue::ConfigQueue(int switch_radix)
	: switchRadix(switch_radix) {}


ConfigQueue::~ConfigQueue()
{
	while (!config_q.empty()) {
		delete config_q.front();
		config_q.pop_front();
	}
}

void ConfigQueue::enqueue(lms_config_ptr schedule)
{
	if (!schedule)
		return;

	lms_config_ptr sched_iter = schedule;
	while (sched_iter) {
		config_q.push_back(new Config(switchRadix, sched_iter->coeff, sched_iter->matching));
		sched_iter = sched_iter->next;
	}
	Free_ConfigList(schedule);

}

Config* ConfigQueue::dequeue()
{
	assert(!config_q.empty());
	Config* config = config_q.front();
	config_q.erase(config_q.begin());
	return config;
}

bool ConfigQueue::isEmpty()
{
	return config_q.empty();
}

int ConfigQueue::getDemandCompletionTime()
{
	int sumDCT = 0;

	for (auto iter = config_q.begin(); iter != config_q.end(); iter++) 
		sumDCT += (*iter)->getTimeDuration();
	
	return sumDCT;
	
}
