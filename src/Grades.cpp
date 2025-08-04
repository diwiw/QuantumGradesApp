#include "Grades.h"
#include "Logger.h"
#include <iostream>
#include <numeric>
#include <algorithm>

void Grades::add(int grade) {
	notes.push_back(grade);
	Logger::getInstance().log(LogLevel::INFO, "[Grades] Added Grade: " + std::to_string(grade));
}

float Grades::getMean() const{
	if (notes.empty()) return 0.0f;
	int sum = std::accumulate(notes.begin(), notes.end(), 0);
	return static_cast<float>(sum) / notes.size();
}

void Grades::print() const {
	for(int g : notes) std::cout << g << " ";
	std::cout << "\n";
}

int Grades::getMax() const{
	return *std::max_element(notes.begin(), notes.end());
}

int Grades::getMin() const{
	return *std::min_element(notes.begin(), notes.end());
}
