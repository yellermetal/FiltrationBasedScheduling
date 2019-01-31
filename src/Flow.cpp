#include "Flow.h"

int Flow::instance_counter = 0;

Flow::Flow(int _size, int _timestamp, int src, int dst)
{
	size = _size;
	remainingSize = _size;
	timestamp = _timestamp;
	source = src;
	destination = dst;
	flowCompletionTime = -1;
	id = instance_counter++;
	markedForScheduling = false;
}

bool Flow::service(int curr_timestamp)
{
	remainingSize--;
	if (!remainingSize && flowCompletionTime == -1) {
		flowCompletionTime = curr_timestamp - timestamp;
		return true;
	}
	return false;
}

void Flow::printToFile(ofstream& log)
{
	log << "flow #" << id << ": size: " << size << ", rem_Size: " << remainingSize << ", arrived: " 
		<< timestamp << ", src\\dst:[" << source << "," << destination << "], FCT: " << flowCompletionTime << endl;
}

int Flow::getTimeWaiting(int curr_timestamp)
{
	return curr_timestamp - timestamp;
}
