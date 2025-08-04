#pragma once
#include <vector>
#include <optional>

/**
 * @brief Utility class for performing basic statistical calculations 
 * (static methods, stateless). 
 *
 * This class is stateless and only contains static helper methods.
 */
class Statistics {
public:
	Statistics() = delete;
	~Statistics() = delete;
	Statistics(const Statistics&) = delete;
	Statistics& operator=(const Statistics&) = delete;

	/**
	 * @brief Calculates the mean (avg) of a set of values.
	 * @param values Vector of integers.
	 * @return The mean as double, or std::mullopt if vector is empty
	 */
	static std::optional<double> calculateMean(const std::vector<int>& values);

	/**
	 * @brief Calculates the minimum value from a set of values.
	 * @param values Vector of integers.
	 * @return Minimum value, or std::nullopt if vector is empty.
	 */
	std::optional<int> calculateMin(const std::vector<int>& values);

	/**
	 * @brief Calculates the maximum value from a set of values.
	 * @param values Vector of integers.
	 * @return Maximum value, or std::nullopt if vector is empty.
	 */
	std::optional<int> calculateMax(const std::vector<int>& values);

	/**
	 * @brief Calculates median of given integers.
	 * @param data Vector of integers.
	 * @return Median value or std::nullopt if data is empty.
	 */
	std::optional<double> calculateMedian(const std::vector<int>& values);

	/**
	 * @brief Calculates standard deviation of given integers.
	 * @param values Vector of integers.
	 * @return Standard deviation or std::nullopt if data is empty.
	 */
	std::optional<double> calculateStdDev(const std::vector<int>& values);
};

