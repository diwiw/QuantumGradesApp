#include "Statistics.hpp"
#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <memory>

namespace qga {

namespace {
    // Local logger source - not a singleton
    static std::shared_ptr<utils::ILogger> s_logger = utils::LoggerFactory::createConsoleLogger(
        "Statistics", LogLevel::Info);
}

std::optional<double> Statistics::calculateMean(const std::vector<int>& values) {
    if (values.empty()) {
        s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate mean: data is empty");
        return std::nullopt;
    }

    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();

    s_logger->log(LogLevel::Info, "[Statistics] Mean calculated: " + std::to_string(mean));
    return mean;
}

std::optional<int> Statistics::calculateMin(const std::vector<int>& values) {
    if (values.empty()) {
        s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate min value: data is empty");
        return std::nullopt;
    }

    int min_val = *std::min_element(values.begin(), values.end());
    s_logger->log(LogLevel::Info, "[Statistics] Min calculated: " + std::to_string(min_val));
    return min_val;
}

std::optional<int> Statistics::calculateMax(const std::vector<int>& values) {
    if (values.empty()) {
        s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate max value: data is empty");
        return std::nullopt;
    }

    int max_val = *std::max_element(values.begin(), values.end());
    s_logger->log(LogLevel::Info, "[Statistics] Max calculated: " + std::to_string(max_val));
    return max_val;
}

std::optional<double> Statistics::calculateMedian(const std::vector<int>& values) {
    if (values.empty()) {
        s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate median: data is empty");
        return std::nullopt;
    }

    std::vector<int> sorted_data = values;
    std::sort(sorted_data.begin(), sorted_data.end());

    double median = 0.0;
    size_t n = sorted_data.size();
    if (n % 2 == 0)
        median = (sorted_data[n / 2 - 1] + sorted_data[n / 2]) / 2.0;
    else
        median = sorted_data[n / 2];

    s_logger->log(LogLevel::Info, "[Statistics] Median calculated: " + std::to_string(median));
    return median;
}

std::optional<double> Statistics::calculateStdDev(const std::vector<int>& values) {
    if (values.size() < 2) {
        s_logger->log(LogLevel::Err, "[Statistics] Cannot calculate stddev: not enough values.");
        return std::nullopt;
    }

    auto mean_opt = calculateMean(values);
    if (!mean_opt.has_value()) return std::nullopt;

    double mean = mean_opt.value();
    double sum_sq = 0.0;
    for (int v : values)
        sum_sq += (v - mean) * (v - mean);

    double std_dev = std::sqrt(sum_sq / (values.size() - 1));
    s_logger->log(LogLevel::Info, "[Statistics] StdDev calculated: " + std::to_string(std_dev));
    return std_dev;
}

} // namespace qga