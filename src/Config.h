
#ifndef CONFIG_H
#define CONFIG_H

#include <assert.h>

class Config
{
public:

	Config(int switch_radix, int time_duration, int* perm_mat);
	~Config();
	int* getPermMatrix();
	int  getTimeDuration();
	void decrementTimeDuration();

private:
	int* permMatrix;
	int switchRadix;
	int timeDuration;
};

#endif //CONFIG_H