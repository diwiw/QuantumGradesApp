#include "Statistics.h"
#include "Logger.h"
#include <algorithm>
#include <cmath>

double Statistics::calculateMean(const std::vector<int>& data) const {
	if(data.empty()) return 0.0;
	double sum = 0;
	for(int value : data) sum += value;
	double mean = sum /data.size();
	Logger::getInstance().log(LogLevel::INFO, "Meancalculated: " + std::to_string(mean));
	return mean;
}

std::optional<int> Statistics::calculateMin(const std::vector<int>& data) const{
	if (data.empty()) return std::nullopt;
	int minVal = *std::min_element(data.begin(), data.end());
	Logger::getInstance().log(LogLevel::INFO, "Min calculate: " + std::to_string(minVal));
	return minVal;
}

std::optional<int> Statistics::calculateMax(const std::vector<int>& data) const {
	if (data.empty()) return std::nullopt;
	int maxVal = *std::max_element(data.begin(), data.end());
	Logger::getInstance().log(LogLevel::INFO, "Max calculated: " + std::to_string(maxVal));
	return maxVal;
}

double Statistics::calculateStdDev(const std::vector<int>& data) const{
	if (data.empty()) return 0.0;
	double mean = calculateMean(data);
	double sumSq = 0;
	for (int value : data) sumSq += (value -mean) * (value - mean);
	double stdDev = std::sqrt(sumSq /data.size());
	Logger::getInstance().log(LogLevel::INFO, "StdDev calculated: " + std::to_string(stdDev));
	return stdDev;
}

