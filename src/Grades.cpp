#include "Grades.h"
#include "Logger.h"
#include "Statistics.h"
#include <iostream>
#include <numeric>
#include <algorithm>

void Grades::add(int grade) {
	notes.push_back(grade);
	Logger::getInstance().log(LogLevel::INFO, "[Grades] Added Grade: " + std::to_string(grade));
}

float Grades::getMean() const {
	if (notes.empty()){
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate mean with empty grades list");        
		return 0.0f;
	}
	auto meanOpt = Statistics::calculateMean(notes);
	if(meanOpt.has_value()){
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated mean: " + std::to_string(meanOpt.value()));
		return static_cast<float>(meanOpt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "Failed to calculate mean");
		return 0.0f;
	}
}

float Grades::getMedian() const {
	if (notes.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate median with empty grades list");
		return 0.0f;
	}

	auto medianOpt = Statistics::calculateMedian(notes);
	if(medianOpt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated median: " + std::to_string(medianOpt.value()));
		return static_cast<float>(medianOpt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate median");
		return 0.0f;
	}
}

float Grades::getStdDev() const {
	if(notes.size() < 2) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Not enough grades to calculate standard deviation");
		return 0.0f;
	}

	auto stddevOpt = Statistics::calculateStdDev(notes);
	if(stddevOpt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated standard deviation: " + std::to_string(stddevOpt.value()));
		return static_cast<float>(stddevOpt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate standard deviation");
		return 0.0f;
	}
}

void Grades::printGrades() const {
	for(int g : notes) std::cout << g << " ";
	std::cout << "\n";
}

void Grades::printSummary() const {
	if (notes.empty()) {
		std::cout << "No grades available.\n";
		return;
	}

	std::cout << "\n=== Statistics ===\n";
	auto mean = Statistics::calculateMean(notes);
	auto median = Statistics::calculateMedian(notes);
	auto stddev = Statistics::calculateStdDev(notes);
	auto maxVal = Statistics::calculateMax(notes);
	auto minVal = Statistics::calculateMin(notes);

	if (mean) std::cout << "Mean: " << mean.value() << "\n";
	if (median) std::cout << "Median: " << median.value() << "\n";
	if (stddev) std::cout << "StdDev: " << stddev.value() << "\n";
	if (maxVal) std::cout << "Max: " << maxVal.value() << "\n";
	if (minVal) std::cout << "Min: " << minVal.value() << "\n";
}

const std::vector<int>& Grades::getNotes() const {
	return notes;
}

int Grades::getMax() const{
	if (notes.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculated max with empty list");
		return 0;
	}

	auto maxOpt = Statistics::calculateMax(notes);
	if(maxOpt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated max grade: " + std::to_string(maxOpt.value()));
		return maxOpt.value();
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate max");
		return 0;
	}
}

int Grades::getMin() const{
	if (notes.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate min with empty list");
		return 0;
	}

	auto minOpt = Statistics::calculateMin(notes);
	if(minOpt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated min grade: " + std::to_string(minOpt.value()));
		return minOpt.value();
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate min");
		return 0;
	}
}
