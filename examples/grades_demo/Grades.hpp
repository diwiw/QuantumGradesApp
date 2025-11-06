/**
 * @file Grades.hpp
 * @brief A simple class to manage and analyze a collection of integer grades.
 */
#pragma once

#include <vector>
#include <string>
#include <memory>

namespace qga {
namespace utils {
    class ILogger; ///< Forward declaration to avoid including the full logger header.
}

/**
 * @class Grades
 * @brief Stores a list of integer grades and provides basic statistics.
 *
 * This class allows the user to add grades, retrieve them, and compute
 * various statistical metrics like mean, median, standard deviation,
 * maximum, and minimum. It also provides simple printing utilities.
 */
class Grades {
private:
    /**
     * @brief Internal logger instance for demo output.
     * Used to log informational messages during Grades operations.
     * Created via LoggerFactory (async SpdLogger).
     */    
    std::shared_ptr<qga::utils::ILogger> logger_;   

    std::vector<int> notes_; ///< Container holding all grades.

public:
    /**
     * @brief Default constructor that initializes the internal logger.
     *
     * Creates an asynchronous logger instance using `LoggerFactory` to handle
     * informational and diagnostic messages for this demo class.
     */
    Grades();

    /// Adds a new grade to the internal list.
    void add(int grade);

    /// Returns the current list of grades.
    const std::vector<int>& getNotes() const;

    /// Computes the arithmetic mean of all grades.
    float getMean() const;

    /// Computes the median of all grades.
    float getMedian() const;

    /// Computes the standard deviation of grades.
    float getStdDev() const;

    /// Retrieves the maximum grade.
    int getMax() const;

    /// Retrieves the minimum grade.
    int getMin() const;

    /// Prints a summary of statistics to standard output.
    void printSummary() const;

    /// Prints all grades to standard output.
    void printGrades() const;
};

} // namespace qga
