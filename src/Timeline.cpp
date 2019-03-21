#include "Timeline.h"

Timeline::Timeline(Timeline_params params)
	: switchRadix(params.switchRadix), currTime(0), flowNumber(0)
{

	for (int time = 0; time < WINDOW_NUM*params.time_window; time += params.time_window) {

		int** SMG_mat = Sparse_Matrix_Generator(switchRadix, params.light_flows_num, params.heavy_flows_num, params.light_range, params.heavy_range);

		int currWindow[2] = { time, time + params.time_window };

		for (int src = 0; src < switchRadix; src++)
			for (int dst = 0; dst < switchRadix; dst++)
				if (SMG_mat[src][dst] > 0) {
					flows.push_back(new Flow(SMG_mat[src][dst], randInt(currWindow), src, dst));
					flowNumber++;
				}

		Free_Sparse_Matrix(SMG_mat, switchRadix);
	}
}

Timeline::Timeline(Timeline & timeline)
{
	*this = timeline;
}

int** Timeline::getDemand()
{

	int** demandMatrix = new int*[switchRadix];
	for (int i = 0; i < switchRadix; ++i)
		demandMatrix[i] = new int[switchRadix]();

	for (auto iter = flows.begin(); iter != flows.end(); ++iter) {
		
		Flow* flow = *iter;
		int src = flow->source;
		int dst = flow->destination;

		assert(src < switchRadix && dst < switchRadix);

		if (flow->markedForScheduling == false &&
			flow->timestamp <= currTime &&
			flow->remainingSize > 0) {
							
				demandMatrix[src][dst] += flow->remainingSize;
				flow->markedForScheduling = true;
		}
	}

	return demandMatrix;
}

void Timeline::serveDemand(Config* config)
{
	int* flow_serviced = new int[switchRadix]();

	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		
		Flow* flow = *iter;
		int src = flow->source;
		int dst = flow->destination;
		int rSize = flow->remainingSize;
		int arrival = flow->timestamp;
		if ((rSize > 0) &&
			(src == config->getPermMatrix()[dst]) &&
			(arrival <= currTime) &&
			(flow_serviced[src] == 0)) {
			if (flow->service(currTime)) {
				flowNumber--;
				if (flowNumber == 0)
					cout << "Timeline has been served!" << endl;
			}
			flow_serviced[src]++;
		}
	}

	delete[] flow_serviced;
}

void Timeline::reset()
{
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		Flow* flow = *iter;
		flow->remainingSize = flow->size;
		flow->flowCompletionTime = -1;
		flow->markedForScheduling = false;
		flowNumber++;
	}

	currTime = 0;
}

bool Timeline::isEmpty()
{
	return flowNumber == 0;
}

void Timeline::printToFile(ofstream& file)
{
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		Flow* flow = *iter;
		flow->printToFile(file);
	}
	file.close();
}


void Timeline::update(int clock)
{
	currTime = clock;
}

int ** FilteredTimeline::getDemand()
{
	int** demandMatrix = new int*[switchRadix];
	for (int i = 0; i < switchRadix; ++i)
		demandMatrix[i] = new int[switchRadix]();

	vector<int> sizes;
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {

		Flow* flow = *iter;
		if (flow->markedForScheduling == false &&
			flow->timestamp <= currTime) {
			sizes.push_back( flow->remainingSize + flow->getTimeWaiting(currTime) );
		}
	}

	nth_element(sizes.begin(), sizes.begin() + sizes.size()/2, sizes.end());
	int median = sizes[sizes.size()/2];


	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		
		Flow* flow = *iter;
		int src = flow->source;
		int dst = flow->destination;
		assert(src < switchRadix && dst < switchRadix);

		if (flow->markedForScheduling == false &&
			flow->timestamp <= currTime &&
			flow->remainingSize + flow->getTimeWaiting(currTime) >= median) {

				demandMatrix[src][dst] += flow->remainingSize;
				flow->markedForScheduling = true;
		}
		
	}

	return demandMatrix;
}
