
#include <string>
#include "Switch.h"
#include "Scheduler.h"
#include "Timeline.h"

#define NUM_SCHED 3

using namespace std;

int main(int argc, char *argv[]) {

	int ind = 1;

	Timeline_params params;

	params.switchRadix = atoi(argv[ind++]);
	int reconfig_penalty = atoi(argv[ind++]);
	params.time_window = atoi(argv[ind++]);
	params.light_flows_num = atoi(argv[ind++]);
	params.heavy_flows_num = atoi(argv[ind++]);
	params.light_range[0] = atoi(argv[ind++]);
	params.light_range[1] = atoi(argv[ind++]);
	params.heavy_range[0] = atoi(argv[ind++]);
	params.heavy_range[1] = atoi(argv[ind++]);

	Clock clock;
	Timeline timeline(params);
	//FilteredTimeline filtered_timeline(timeline, reconfig_penalty);
	Scheduler schedulers[NUM_SCHED] = { Lumos(params.switchRadix, reconfig_penalty, clock),
										Solstice(params.switchRadix, reconfig_penalty, clock), 
										Eclipse(params.switchRadix, reconfig_penalty, clock) };

	clock.attach(timeline);
	
	for (int sched = 0; sched < NUM_SCHED; sched++) {

		clock.reset();
		clock.attach(schedulers[sched]);

		Switch optical_circuit_switch(params.switchRadix, timeline, schedulers[sched]);
		clock.attach(optical_circuit_switch);

		while (!timeline.isEmpty())
			clock.tick();

		clock.detach(schedulers[sched]);
		clock.detach(optical_circuit_switch);
		timeline.printToFile(schedulers[sched].openFile());
	}
	
	return 0;
}