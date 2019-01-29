#pragma once


#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "Observer.h"
#include <vector>
using namespace std;

class Observable
{
public:
	virtual ~Observable() {};

	void attach(Observer& observer);
	void detach(Observer& observer);
	void notifyAllObservers();

private:
	vector<Observer> observers;

};

#endif //OBSERVABLE_H