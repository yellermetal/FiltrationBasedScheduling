
#ifndef TIMELINE_H
#define TIMELINE_H


#include "Demand.h"
#include "Flow.h"
#include "SMG.h"
#include <vector>

#define WINDOW_NUM 30

typedef struct _Timeline_params {
	int switchRadix;
	int time_window;
	int light_flows_num;
	int heavy_flows_num;
	int light_range[2];
	int heavy_range[2];
} Timeline_params;

using namespace std;

class Timeline :
	public Demand
{
public:
	Timeline(Timeline_params);
	Timeline(Timeline& timeline);
	int** getDemand();
	void serveDemand(Config* config);
	void reset();
	bool isEmpty();
	void printToFile(ofstream& file);
	void update(int clock);

protected:
	int switchRadix;
	int currTime;
	vector<Flow*> flows;

private:
	int flowNumber;

};

class FilteredTimeline :
	public Timeline
{
public:
	FilteredTimeline(Timeline_params params, int _threshold) :
		Timeline(params), threshold(_threshold) {};
	FilteredTimeline(Timeline& timeline, int _threshold):
		Timeline(timeline), threshold(_threshold) {};

	int** getDemand();

private:
	int threshold;
};

#endif //TIMELINE_H
