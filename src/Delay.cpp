#include "Delay.h"


Delay::Delay(int _delay) : delay(_delay) {}

int Delay::getDelay()
{
	return delay;
}

void Delay::setDelay(int runtime)
{
	delay = TRIVIAL_DELAY;
}

void Delay::update(Observable& observable)
{
	delay--;
	assert(delay >= 0);
}

AdaptiveDelay::AdaptiveDelay(int _switchRadix, int _reconfig_penalty, ConfigQueue & _configQueue):
	Delay(0), lastRuntime(0), switchRadix(_switchRadix), 
	reconfig_penalty(_reconfig_penalty), configQueue(_configQueue) {}

void AdaptiveDelay::setDelay(int runtime)
{
	int currRuntime = runtime;
	int configQueueDemandCompletionTime = configQueue.getDemandCompletionTime();

	delay = max(switchRadix * reconfig_penalty, configQueueDemandCompletionTime - lastRuntime);
	lastRuntime = currRuntime;
}
