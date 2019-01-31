
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

	Timeline timeline(params);
	//FilteredTimeline filtered_timeline(timeline, reconfig_penalty);

	Scheduler* schedulers[NUM_SCHED] = { new Lumos(params.switchRadix, reconfig_penalty), 
										new Solstice(params.switchRadix, reconfig_penalty),
										new Eclipse(params.switchRadix, reconfig_penalty) };
	
	freopen( "console.txt", "w", stdout );

	for (int sched = 0; sched < NUM_SCHED; sched++) {

		Switch optical_circuit_switch(params.switchRadix, timeline, schedulers[sched]);
		schedulers[sched]->useAdaptiveSchedulingDelay(true);

		for (int clock = 0; !timeline.isEmpty(); clock++) {
			timeline.update(clock);
			schedulers[sched]->update(clock);
			optical_circuit_switch.update(clock);

		}

		ofstream file;
		schedulers[sched]->openFile(file);
		assert(file.is_open());
		timeline.printToFile(file);
		timeline.reset();
		delete schedulers[sched];
	}
	
	return 0;
}
