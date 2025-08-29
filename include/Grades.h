/**
 * @file Grades.h
 * @brief A simple class to manage and analyze a collection of integer grades.
 */
#pragma once

#include <vector>
#include <string>

/**
 * @class Grades
 * @brief Stores a list of integer grades and provides basic statistics.
 *
 * This class allows the user to add grades, retrieve them, and compute
 * various statistical metrics like mean, median, standard deviation,
 * maximum, and minimum. It also provides simple printing utilities.
 */
class Grades{
private:
	std::vector<int> notes_; ///< Container holding all grades.

public:
	/**
     * @brief Adds a new grade to the internal list.
     * @param grade The grade to be added (assumed to be a valid integer).
     */
	void add(int grade);

	/**
     * @brief Returns the current list of grades.
     * @return A const reference to the vector of grades.
     */
	const std::vector<int>& getNotes() const;

	/**
     * @brief Computes the arithmetic mean of all grades.
     * @return Mean as a float.
     */
	float getMean() const;

	/**
     * @brief Computes the median of all grades.
     * @return Median as a float.
     */
	float getMedian() const;

	 /**
     * @brief Computes the standard deviation of grades.
     * @return Standard deviation as a float.
     */
	float getStdDev() const;

	/**
     * @brief Retrieves the maximum grade.
     * @return Maximum grade as an integer.
     */
	int getMax() const;

	/**
     * @brief Retrieves the minimum grade.
     * @return Minimum grade as an integer.
     */
	int getMin() const;

	/**
     * @brief Prints a summary of statistics to standard output.
     */
	void printSummary() const;

	 /**
     * @brief Prints all grades to standard output.
     */
	void printGrades() const;

};
