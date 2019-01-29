#include "Observable.h"


void Observable::attach(Observer & observer)
{
	observers.push_back(observer);
}

void Observable::detach(Observer & observer)
{
	for (auto iter = observers.begin(); iter != observers.end(); iter++)
		if (*iter == observer) {
			observers.erase(iter);
			return;
		}
}

void Observable::notifyAllObservers()
{
	for (auto iter = observers.begin(); iter != observers.end(); iter++)
		iter->update(*this);

}
