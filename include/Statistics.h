#pragma once
#include <vector>
#include <string>
#include <optional>

class Statistics {
public:
	Statistics() = default;

	double calculateMean(const std::vector<int>& data) const;
	double calculateMedian(std::vector<int> data) const;
	std::optional<int> calculateMin(const std::vector<int>& data) const;
	std::optional<int> calculateMax(const std::vector<int>& data) const;
	double calculateStdDev(const std::vector<int>& data) const;
};

