#include "Config.h"

Config::Config(int switch_radix, int time_duration, int * perm_mat) 
{

	switchRadix = switch_radix;
	timeDuration = time_duration;
	permMatrix = new int[switch_radix]();

	for (int i = 0; i < switchRadix; i++) {
		permMatrix[i] = perm_mat[i];
	}

	bool in_list;
	int empty_slot;
	for (int elem = 0; elem < switchRadix; elem++) {

		in_list = false;
		empty_slot = -1;
		for (int j = 0; j < switchRadix; j++) {
			if (permMatrix[j] == elem)
				in_list = true;
			if (permMatrix[j] == -1)
				empty_slot = j;
		}

		if (!in_list) {
			assert(empty_slot != -1);
			permMatrix[empty_slot] = elem;
		}
	}
}

Config::~Config() {
	delete[] permMatrix;
}

int * Config::getPermMatrix()
{
	return permMatrix;
}

int Config::getTimeDuration()
{
	return timeDuration;
}

void Config::decrementTimeDuration()
{
	timeDuration--;
}
