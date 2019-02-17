
#include <string>
#include "Switch.h"
#include "Scheduler.h"
#include "Timeline.h"

#define NUM_SCHED 2

using namespace std;

const string currentDateTime();

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

	bool adaptive = atoi(argv[ind++]);
	bool useFilter = atoi(argv[ind++]);
	int threshold = atoi(argv[ind++]);

	Timeline basic_timeline(params);
	FilteredTimeline filtered_timeline(basic_timeline, threshold);

	Timeline& timeline = useFilter ? filtered_timeline : basic_timeline;

	Scheduler* schedulers[NUM_SCHED] = { new Lumos(params.switchRadix, reconfig_penalty), 
										new Solstice(params.switchRadix, reconfig_penalty),
										};//new Eclipse(params.switchRadix, reconfig_penalty) };
	
	cout.rdbuf(nullptr);
	//freopen( "console.txt", "w", stdout );

	for (int sched = 0; sched < NUM_SCHED; sched++) {

		Switch optical_circuit_switch(params.switchRadix, reconfig_penalty, timeline, schedulers[sched]);
		schedulers[sched]->useAdaptiveSchedulingDelay(adaptive);


		for (int clock = 0; !timeline.isEmpty(); clock++) {
			timeline.update(clock);
			schedulers[sched]->update(clock);
			optical_circuit_switch.update(clock);

		}

		ofstream file;

		string prefix = "./Results/";
		string suffix = ".txt";
		string filename = prefix + schedulers[sched]->getName() + currentDateTime() + suffix;

		file.open(filename.c_str());
		assert(file.is_open());
		timeline.printToFile(file);
		timeline.reset();


		delete schedulers[sched];

	}
	
	return 0;
}

const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%F.%H-%M-%S", &tstruct);

    return buf;
}
