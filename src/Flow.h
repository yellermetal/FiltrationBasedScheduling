
#ifndef FLOW_H
#define FLOW_H

#include <iostream>
#include <fstream>

using namespace std;

class Flow {
public:
	
	Flow(int size, int timestamp, int src, int dst);
	bool service(int curr_timestamp);
	void printToFile(ofstream& log);
	int getTimeWaiting(int curr_timestamp);

	int id;		
	int size;      
	int remainingSize;
	int timestamp;   
	int source;     
	int destination; 
	int flowCompletionTime;
	bool markedForScheduling;

private:
	static int instance_counter;

};

#endif //FLOW_H