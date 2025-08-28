#include "Grades.h"
#include "Logger.h"
#include "Statistics.h"
#include <iostream>
#include <numeric>
#include <algorithm>

void Grades::add(int grade) {
	notes_.push_back(grade);
	Logger::getInstance().log(LogLevel::INFO, "[Grades] Added Grade: " + std::to_string(grade));
}

float Grades::getMean() const {
	if (notes_.empty()){
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate mean with empty grades list");        
		return 0.0f;
	}
	auto mean_opt = Statistics::calculateMean(notes_);
	if(mean_opt.has_value()){
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated mean: " + std::to_string(mean_opt.value()));
		return static_cast<float>(mean_opt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "Failed to calculate mean");
		return 0.0f;
	}
}

float Grades::getMedian() const {
	if (notes_.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate median with empty grades list");
		return 0.0f;
	}

	auto median_opt = Statistics::calculateMedian(notes_);
	if(median_opt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated median: " + std::to_string(median_opt.value()));
		return static_cast<float>(median_opt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate median");
		return 0.0f;
	}
}

float Grades::getStdDev() const {
	if(notes_.size() < 2) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Not enough grades to calculate standard deviation");
		return 0.0f;
	}

	auto stddev_opt = Statistics::calculateStdDev(notes_);
	if(stddev_opt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated standard deviation: " + std::to_string(stddev_opt.value()));
		return static_cast<float>(stddev_opt.value());
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate standard deviation");
		return 0.0f;
	}
}

void Grades::printGrades() const {
	for(int g : notes_) std::cout << g << " ";
	std::cout << "\n";
}

void Grades::printSummary() const {
	if (notes_.empty()) {
		std::cout << "No grades available.\n";
		return;
	}

	std::cout << "\n=== Statistics ===\n";
	auto mean = Statistics::calculateMean(notes_);
	auto median = Statistics::calculateMedian(notes_);
	auto stddev = Statistics::calculateStdDev(notes_);
	auto max_val = Statistics::calculateMax(notes_);
	auto min_val = Statistics::calculateMin(notes_);

	if (mean) std::cout << "Mean: " << mean.value() << "\n";
	if (median) std::cout << "Median: " << median.value() << "\n";
	if (stddev) std::cout << "StdDev: " << stddev.value() << "\n";
	if (max_val) std::cout << "Max: " << max_val.value() << "\n";
	if (min_val) std::cout << "Min: " << min_val.value() << "\n";
}

const std::vector<int>& Grades::getNotes() const {
	return notes_;
}

int Grades::getMax() const{
	if (notes_.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculated max with empty list");
		return 0;
	}

	auto max_opt = Statistics::calculateMax(notes_);
	if(max_opt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated max grade: " + std::to_string(max_opt.value()));
		return max_opt.value();
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate max");
		return 0;
	}
}

int Grades::getMin() const{
	if (notes_.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[Grades] Attempted to calculate min with empty list");
		return 0;
	}

	auto min_opt = Statistics::calculateMin(notes_);
	if(min_opt.has_value()) {
		Logger::getInstance().log(LogLevel::INFO, "[Grades] Calculated min grade: " + std::to_string(min_opt.value()));
		return min_opt.value();
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[Grades] Failed to calculate min");
		return 0;
	}
}
