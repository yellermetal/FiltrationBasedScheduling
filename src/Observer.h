
#ifndef OBSERVER_H
#define OBSERVER_H

class Observable;

class Observer
{
public:

	Observer();
	virtual ~Observer() {};

	bool operator==(const Observer& rhs);
	virtual void update(Observable& observable) {};

private:
	static int instace_counter;
	int id;
};

#endif //OBSERVER_H
