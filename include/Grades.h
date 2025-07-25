#ifndef GRADES_H
#define GRADES_H

#include <vector>
#include <string>

class Grades{
private:
	std::vector<int> notes;

public:
	void add(int grade);
	float getMean() const;
	int getMax() const;
	int getMin() const;
	void print() const;

};

#endif
