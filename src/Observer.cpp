#include "Observer.h"

int Observer::instace_counter = 0;

Observer::Observer() {
	id = instace_counter++;
}

bool Observer::operator==(const Observer & rhs)
{
	return id == rhs.id;
}
