#include "core/Statistics.hpp"
#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <numeric>
#include <stdexcept>

namespace qga::core
{

    namespace
    {
        // Local logger – NOT a singleton (unit-test friendly)
        static std::shared_ptr<utils::ILogger> s_logger =
            utils::LoggerFactory::createConsoleLogger("Statistics", LogLevel::Info);
    } // namespace

    // ================================================================
    // BASIC STATISTICS
    // ================================================================

    std::optional<double> Statistics::calculateMean(const std::vector<int>& values)
    {
        if (values.empty())
        {
            s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate mean: data is empty");
            return std::nullopt;
        }

        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        double mean = sum / values.size();

        s_logger->log(LogLevel::Info, "[Statistics] Mean calculated: " + std::to_string(mean));
        return mean;
    }

    std::optional<int> Statistics::calculateMin(const std::vector<int>& values)
    {
        if (values.empty())
        {
            s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate min: data is empty");
            return std::nullopt;
        }

        int min_val = *std::min_element(values.begin(), values.end());
        s_logger->log(LogLevel::Info, "[Statistics] Min calculated: " + std::to_string(min_val));
        return min_val;
    }

    std::optional<int> Statistics::calculateMax(const std::vector<int>& values)
    {
        if (values.empty())
        {
            s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate max: data is empty");
            return std::nullopt;
        }

        int max_val = *std::max_element(values.begin(), values.end());
        s_logger->log(LogLevel::Info, "[Statistics] Max calculated: " + std::to_string(max_val));
        return max_val;
    }

    std::optional<double> Statistics::calculateMedian(const std::vector<int>& values)
    {
        if (values.empty())
        {
            s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate median: data is empty");
            return std::nullopt;
        }

        std::vector<int> sorted = values;
        std::sort(sorted.begin(), sorted.end());

        size_t n = sorted.size();
        double median = (n % 2 == 0)
                            ? (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0
                            : sorted[n / 2];

        s_logger->log(LogLevel::Info,
                      "[Statistics] Median calculated: " + std::to_string(median));
        return median;
    }

    std::optional<double> Statistics::calculateStdDev(const std::vector<int>& values)
    {
        if (values.size() < 2)
        {
            s_logger->log(LogLevel::Err,
                          "[Statistics] Cannot calculate stddev: not enough values");
            return std::nullopt;
        }

        auto mean_opt = calculateMean(values);
        if (!mean_opt)
            return std::nullopt;

        double mean = *mean_opt;
        double sum_sq = 0.0;

        for (int v : values)
            sum_sq += (v - mean) * (v - mean);

        double stddev = std::sqrt(sum_sq / (values.size() - 1));

        s_logger->log(LogLevel::Info,
                      "[Statistics] StdDev calculated: " + std::to_string(stddev));
        return stddev;
    }

    // ================================================================
    // FINANCIAL METRICS
    // ================================================================

    /**
     * Max Drawdown
     */
    double Statistics::maxDrawdown(const std::vector<double>& equity)
    {
        if (equity.size() < 2)
            return 0.0;

        double peak = equity[0];
        double max_dd = 0.0;

        for (double v : equity)
        {
            peak = std::max(peak, v);
            double dd = (peak - v) / peak;
            max_dd = std::max(max_dd, dd);
        }

        s_logger->log(LogLevel::Info, "[Statistics] MaxDrawdown calculated: " +
                                          std::to_string(max_dd));
        return max_dd;
    }

    /**
     * CAGR = (final_value / initial_value)^(1/years) - 1
     */
    double Statistics::cagr(const std::vector<double>& equity, double periods_per_year)
    {
        if (equity.size() < 2)
            return 0.0;
        // TODO(v1.2.0–v1.9.9):
        //   Extract this validation into a dedicated helper method
        //   (e.g. validate periods_per_year()) to improve SRP and keep
        //   financial metrics module clean and uniform.
        //
        //   Minimal inline validation is kept here temporarily to ensure
        //   runtime stability before the full StatisticsFinance refactor.
        if (periods_per_year <= 0.0 || !std::isfinite(periods_per_year))
        {
            s_logger->log(LogLevel::Err,
                        "[Statistics] Invalid periods_per_year in CAGR");
            return 0.0;
        }
        double start = equity.front();
        double end = equity.back();
        double years = (equity.size() - 1) / periods_per_year;

        if (start <= 0.0 || years <= 0.0)
            return 0.0;

        double result = std::pow(end / start, 1.0 / years) - 1.0;

        s_logger->log(LogLevel::Info, "[Statistics] CAGR calculated: " +
                                          std::to_string(result));
        return result;
    }

    /**
     * Sharpe Ratio
     */
    double Statistics::sharpeRatio(const std::vector<double>& returns,
                                   double risk_free_annual,
                                   double periods_per_year)
    {
        if (returns.size() < 2)
            return 0.0;
        // TODO(v1.2.0–v1.9.9):
        //   Extract this validation into a dedicated helper method
        //   (e.g. validateperiods_per_year()) to improve SRP and keep
        //   financial metrics module clean and uniform.
        //
        //   Minimal inline validation is kept here temporarily to ensure
        //   runtime stability before the full StatisticsFinance refactor.
        if (periods_per_year <= 0.0 || !std::isfinite(periods_per_year))
        {
            s_logger->log(LogLevel::Err,
                        "[Statistics] Invalid periods_per_year in Sharpe Ratio");
            return 0.0;
        }

        // Convert annual risk-free to per-period
        double rf = risk_free_annual / periods_per_year;

        double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();

        // stddev
        double sumsq = 0.0;
        for (double r : returns)
            sumsq += (r - mean) * (r - mean);
        double stddev = std::sqrt(sumsq / (returns.size() - 1));

        if (stddev == 0.0)
            return 0.0;

        double sharpe = (mean - rf) / stddev;

        s_logger->log(LogLevel::Info,
                      "[Statistics] Sharpe Ratio calculated: " + std::to_string(sharpe));
        return sharpe;
    }

    /**
     * Sortino Ratio
     */
    double Statistics::sortinoRatio(const std::vector<double>& returns,
                                    double risk_free_annual,
                                    double periods_per_year)
    {
        if (returns.size() < 2)
            return 0.0;
        // TODO(v1.2.0–v1.9.9):
        //   Extract this validation into a dedicated helper method
        //   (e.g. validate periods_per_year()) to improve SRP and keep
        //   financial metrics module clean and uniform.
        //
        //   Minimal inline validation is kept here temporarily to ensure
        //   runtime stability before the full StatisticsFinance refactor.
        if (periods_per_year <= 0.0 || !std::isfinite(periods_per_year))
        {
            s_logger->log(LogLevel::Err,
                        "[Statistics] Invalid periods_per_year in Sortino Ratio");
            return 0.0;
        }
        double rf = risk_free_annual / periods_per_year;

        std::vector<double> downside;
        downside.reserve(returns.size());

        for (double r : returns)
            if (r < rf)
                downside.push_back(r - rf);

        if (downside.empty())
            return 0.0;

        double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();

        double sumsq = 0.0;
        for (double d : downside)
            sumsq += d * d;
        double downside_dev = std::sqrt(sumsq / downside.size());

        if (downside_dev == 0.0)
            return 0.0;

        double sortino = (mean - rf) / downside_dev;

        s_logger->log(LogLevel::Info,
                      "[Statistics] Sortino Ratio calculated: " + std::to_string(sortino));
        return sortino;
    }

    /**
     * Hit Ratio
     */
    double Statistics::hitRatio(const std::vector<double>& returns)
    {
        if (returns.empty())
            return 0.0;

        size_t wins = 0;
        for (double r : returns)
            if (r > 0)
                wins++;

        double ratio = static_cast<double>(wins) / returns.size();

        s_logger->log(LogLevel::Info,
                      "[Statistics] Hit Ratio calculated: " + std::to_string(ratio));
        return ratio;
    }

} // namespace qga::core
