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

void Flow::printToFile(FILE * log)
{
	fprintf(log, "flow #%d: size: %d, rem_Size: %d, arrived: %d, src\\dst:[%d,%d], FCT: %d\n",
		id, size, remainingSize, timestamp, source, destination, flowCompletionTime);
}

int Flow::getTimeWaiting(int curr_timestamp)
{
	return curr_timestamp - timestamp;
}
