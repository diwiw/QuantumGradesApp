/**
 * @file Statistics.hpp
 * @brief Utility class for statistical and financial performance calculations.
 *
 * This class is stateless and contains only static helper methods.
 * 
 * TODO(v1.2.0–v1.9.9):
 *   - Split into two modules:
 *       StatisticsBasic (mean, stddev, median)
 *       StatisticsFinance (Sharpe, Sortino, CAGR, MDD, HitRatio)
 *
 * TODO(v1.2.0–v1.9.9):
 *   - Convert int-based metrics to double-based or templated versions
 *     for full type consistency.
 */

#pragma once

#include <optional>
#include <vector>

namespace qga::core
{

class Statistics
{
  public:
    Statistics() = delete;
    ~Statistics() = delete;
    Statistics(const Statistics&) = delete;
    Statistics& operator=(const Statistics&) = delete;

    // =========================================
    // Basic statistics
    // =========================================

    /**
     * @brief Computes the arithmetic mean of integer values.
     *
     * @param values Vector of integers.
     * @return Mean value, or std::nullopt if vector is empty.
     *
     * @note TODO: Convert to double-based or template version.
     */
    static std::optional<double> calculateMean(const std::vector<int>& values);

    /**
     * @brief Returns the minimum element in the vector.
     *
     * @param values Vector of integers.
     * @return Minimum value or std::nullopt if vector is empty.
     */
    static std::optional<int> calculateMin(const std::vector<int>& values);

    /**
     * @brief Returns the maximum element in the vector.
     *
     * @param values Vector of integers.
     * @return Maximum value or std::nullopt if vector is empty.
     */
    static std::optional<int> calculateMax(const std::vector<int>& values);

    /**
     * @brief Computes the median of integer values.
     *
     * @param values Vector of integers.
     * @return Median value, or std::nullopt if vector is empty.
     */
    static std::optional<double> calculateMedian(const std::vector<int>& values);

    /**
     * @brief Computes standard deviation of integer values.
     *
     * @param values Vector of integers.
     * @return Standard deviation or std::nullopt if vector is empty.
     */
    static std::optional<double> calculateStdDev(const std::vector<int>& values);

    // =========================================
    // Financial performance metrics
    // =========================================

    /**
     * @brief Computes Maximum Drawdown (MDD) of an equity curve.
     *
     * @param equity Sequence of equity or price values.
     * @return Maximum drawdown in range [0.0, 1.0].
     */
    static double maxDrawdown(const std::vector<double>& equity);

    /**
     * @brief Computes Compound Annual Growth Rate (CAGR).
     *
     * @param equity Time series of portfolio values.
     * @param periods_per_year Sampling frequency (e.g., 252 for daily).
     * @return CAGR as decimal, or 0.0 if invalid.
     */
    static double cagr(const std::vector<double>& equity,
                       double periods_per_year);

    /**
     * @brief Computes annualized Sharpe Ratio.
     *
     * @param returns Periodic returns.
     * @param risk_free_annual Annual risk-free rate.
     * @param periods_per_year Sampling frequency.
     * @return Sharpe Ratio (0.0 if stddev == 0 or input empty).
     */
    static double sharpeRatio(const std::vector<double>& returns,
                              double risk_free_annual,
                              double periods_per_year);

    /**
     * @brief Computes annualized Sortino Ratio.
     *
     * @param returns Periodic returns.
     * @param risk_free_annual Annual risk-free rate.
     * @param periods_per_year Sampling frequency.
     * @return Sortino Ratio, 0.0 if downside deviation is zero.
     */
    static double sortinoRatio(const std::vector<double>& returns,
                               double risk_free_annual,
                               double periods_per_year);

    /**
     * @brief Computes Hit Ratio (win rate).
     *
     * @param returns List of individual trade results.
     * @return Fraction of positive trades in [0.0, 1.0].
     */
    static double hitRatio(const std::vector<double>& returns);
};

} // namespace qga::core
