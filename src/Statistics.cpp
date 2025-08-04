#include "Statistics.h"
#include "Logger.h"
#include <algorithm>
#include <cmath>
#include <numeric>

std::optional<double> Statistics::calculateMean(const std::vector<int>& values)  {
	if(values.empty()) {
		Logger::getInstance().log(LogLevel::ERROR, "[Statistics] Cannot calculate mean: data is empty");
		return std::nullopt;
	}

	double sum = 0;
	for(int v : values) sum += v;
	double mean = sum /values.size();
	
	Logger::getInstance().log(LogLevel::INFO, "[Statistics] Mean calculated: " + std::to_string(mean));
	return mean;
}

std::optional<int> Statistics::calculateMin(const std::vector<int>& values) {
	if (values.empty()) {
		Logger::getInstance().log(LogLevel::ERROR, "[Statistics] Cannot calculate min value: data is empty");
		return std::nullopt;
	}
	int minVal = *std::min_element(values.begin(), values.end());
	Logger::getInstance().log(LogLevel::INFO, "[Statistics] Min calculate: " + std::to_string(minVal));
	return minVal;
}

std::optional<int> Statistics::calculateMax(const std::vector<int>& values) {
	if (values.empty()) {
		Logger::getInstance().log(LogLevel::ERROR, "[Statistics] Cannot calculate max value: data ist empty");
		return std::nullopt;
	}
	int maxVal = *std::max_element(values.begin(), values.end());
	Logger::getInstance().log(LogLevel::INFO, "[Statistics] Max calculated: " + std::to_string(maxVal));
	return maxVal;
}

std::optional<double> Statistics::calculateMedian(const std::vector<int>& values) {
	if(values.empty()){
		Logger::getInstance().log(LogLevel::ERROR, "[Statistics] Cannot calculate median: data is empty");
		return std::nullopt;
	}

	std::vector<int> sortedData = values;
	std::sort(sortedData.begin(), sortedData.end());

	double median;
	if(sortedData.size() % 2 == 0) {
		median = (sortedData[sortedData.size()/2 -1] + sortedData[sortedData.size()/2]) / 2.0;
	} else {
		median = sortedData[sortedData.size()/2];
	}

	Logger::getInstance().log(LogLevel::INFO, "[Statistics] Median calculated: " + std::to_string(median));
	return median;
}

std::optional<double> Statistics::calculateStdDev(const std::vector<int>& values) {
	if (values.size() < 2 ) {
		Logger::getInstance().log(LogLevel::ERROR, "[Statistics] Cannot calculate stddev: not enough values.");
	       	return std::nullopt;
	}

	auto meanOpt = calculateMean(values);
	if(!meanOpt.has_value()) return std::nullopt;

	double mean = meanOpt.value();
	double sumSq = 0.0;

	for (int v : values) sumSq += (v -mean) * (v - mean);
	double stdDev = std::sqrt(sumSq / (values.size()-1));
	Logger::getInstance().log(LogLevel::INFO, "[Statistics] StdDev calculated: " + std::to_string(stdDev));
	return stdDev;
}

