
#ifndef CONFIG_QUEUE_H
#define CONFIG_QUEUE_H

#include <list>
#include "lms_structs.h"
#include "Resources.h"
#include "Config.h"

class ConfigQueue
{
public:

	ConfigQueue(int switch_radix);
	~ConfigQueue();
	void enqueue(lms_config_ptr schedule);
	Config* dequeue();
	bool isEmpty();
	int getDemandCompletionTime(int reconfig_penalty);


private:
	int switchRadix;
	std::list<Config*> config_q;
};

#endif //CONFIG_QUEUE_H

