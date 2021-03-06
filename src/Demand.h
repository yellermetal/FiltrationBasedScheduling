
#ifndef DEMAND_H
#define DEMAND_H

#include "Config.h"

class Demand
{
public:
	virtual ~Demand() {};

	virtual int** getDemand() = 0;
	virtual void serveDemand(Config* config) = 0;
};

#endif //DEMAND_H