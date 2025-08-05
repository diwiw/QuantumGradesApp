#pragma once

#include <vector>
#include <string>

class Grades{
private:
	std::vector<int> notes;

public:
	void add(int grade);
	const std::vector<int>& getNotes() const;
	float getMean() const;
	float getMedian() const;
	float getStdDev() const;
	int getMax() const;
	int getMin() const;
	void printSummary() const;
	void printGrades() const;

};
