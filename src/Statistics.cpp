#include "Statistics.hpp"
#include "utils/Logger.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

namespace qga {

std::optional<double> Statistics::calculateMean(const std::vector<int>& values)  {
	if(values.empty()) {
		qga::utils::Logger::getInstance().log(qga::utils::LogLevel::ERROR, "[Statistics] Cannot calculate mean: data is empty");
		return std::nullopt;
	}

	double sum = 0;
	for(int v : values) sum += v;
	double mean = sum /values.size();

	qga::utils::Logger::getInstance().log(qga::utils::LogLevel::INFO, "[Statistics] Mean calculated: " + std::to_string(mean));
	return mean;
}

std::optional<int> qga::Statistics::calculateMin(const std::vector<int>& values) {
	if (values.empty()) {
		qga::utils::Logger::getInstance().log(qga::utils::LogLevel::ERROR, "[Statistics] Cannot calculate min value: data is empty");
		return std::nullopt;
	}
	int min_val = *std::min_element(values.begin(), values.end());
	qga::utils::Logger::getInstance().log(qga::utils::LogLevel::INFO, "[Statistics] Min calculate: " + std::to_string(min_val));
	return min_val;
}

std::optional<int> qga::Statistics::calculateMax(const std::vector<int>& values) {
	if (values.empty()) {
		qga::utils::Logger::getInstance().log(qga::utils::LogLevel::ERROR, "[Statistics] Cannot calculate max value: data ist empty");
		return std::nullopt;
	}
	int max_val = *std::max_element(values.begin(), values.end());
	qga::utils::Logger::getInstance().log(qga::utils::LogLevel::INFO, "[Statistics] Max calculated: " + std::to_string(max_val));
	return max_val;
}

std::optional<double> qga::Statistics::calculateMedian(const std::vector<int>& values) {
	if(values.empty()){
		qga::utils::Logger::getInstance().log(qga::utils::LogLevel::ERROR, "[Statistics] Cannot calculate median: data is empty");
		return std::nullopt;
	}

	std::vector<int> sorted_data = values;
	std::sort(sorted_data.begin(), sorted_data.end());

	double median;
	if(sorted_data.size() % 2 == 0) {
		median = (sorted_data[sorted_data.size()/2 -1] + sorted_data[sorted_data.size()/2]) / 2.0;
	} else {
		median = sorted_data[sorted_data.size()/2];
	}

	qga::utils::Logger::getInstance().log(qga::utils::LogLevel::INFO, "[Statistics] Median calculated: " + std::to_string(median));
	return median;
}

std::optional<double> qga::Statistics::calculateStdDev(const std::vector<int>& values) {
	if (values.size() < 2 ) {
		qga::utils::Logger::getInstance().log(qga::utils::LogLevel::ERROR, "[Statistics] Cannot calculate stddev: not enough values.");
	       	return std::nullopt;
	}

	auto mean_opt = calculateMean(values);
	if(!mean_opt.has_value()) return std::nullopt;

	double mean = mean_opt.value();
	double sum_sq = 0.0;

	for (int v : values) sum_sq += (v -mean) * (v - mean);
	double std_dev = std::sqrt(sum_sq / (values.size()-1));
	qga::utils::Logger::getInstance().log(qga::utils::LogLevel::INFO, "[Statistics] StdDev calculated: " + std::to_string(std_dev));
	return std_dev;
}

} // namespace qga
