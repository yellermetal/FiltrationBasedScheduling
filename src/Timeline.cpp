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

	if (flows.empty()) return nullptr;

	int** demandMatrix = new int*[switchRadix];
	for (int i = 0; i < switchRadix; ++i)
		demandMatrix[i] = new int[switchRadix]();

	int src, dst;
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		
		src = (*iter)->source;
		dst = (*iter)->destination;

		assert(src < switchRadix && dst < switchRadix);

		if ((*iter)->markedForScheduling == false &&
			(*iter)->timestamp <= currTime && 
			(*iter)->remainingSize > 0) {
							
				demandMatrix[src][dst] += (*iter)->remainingSize;
				(*iter)->markedForScheduling = true;
		}
	}

	return demandMatrix;
}

void Timeline::serveDemand(Config* config)
{
	int* flow_serviced = new int[switchRadix]();

	int src, dst, rSize, arrival;
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		
		src = (*iter)->source;
		dst = (*iter)->destination;
		rSize = (*iter)->remainingSize;
		arrival = (*iter)->timestamp;
		if ((rSize > 0) &&
			(src == config->getPermMatrix()[dst]) &&
			(arrival <= currTime) &&
			(flow_serviced[src] == 0)) {
			if ((*iter)->service(currTime))
				flowNumber--;
			flow_serviced[src] = 1;
		}
	}

	delete[] flow_serviced;
}

void Timeline::reset()
{
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		(*iter)->remainingSize = (*iter)->size;
		(*iter)->flowCompletionTime = -1;
		(*iter)->markedForScheduling = false;
		flowNumber++;
	}
}

bool Timeline::isEmpty()
{
	return flowNumber == 0;
}

void Timeline::printToFile(FILE * log)
{
	for (auto iter = flows.begin(); iter != flows.end(); iter++)
		(*iter)->printToFile(log);
}


void Timeline::update(Observable& observable)
{
	Clock& clock = (Clock &)(observable);
	currTime = clock.time();
}

int ** FilteredTimeline::getDemand()
{
	if (flows.empty()) return nullptr;

	int** demandMatrix = new int*[switchRadix];
	for (int i = 0; i < switchRadix; ++i)
		demandMatrix[i] = new int[switchRadix]();

	int src, dst;
	for (auto iter = flows.begin(); iter != flows.end(); iter++) {
		
		src = (*iter)->source;
		dst = (*iter)->destination;
		assert(src < switchRadix && dst < switchRadix);

		if ((*iter)->markedForScheduling == false &&
			(*iter)->timestamp <= currTime &&
			(*iter)->remainingSize + (*iter)->getTimeWaiting(currTime) >= threshold) {

				demandMatrix[src][dst] += (*iter)->remainingSize;
				(*iter)->markedForScheduling = true;
		}
		
	}

	return demandMatrix;
}
