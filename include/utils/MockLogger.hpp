/**
 * @file MockLogger.hpp
 * @brief Test double for ILogger. Captures logs in memory for inspection in unit tests.
 *
 * This mock logger is used to verify that certain logging actions took place.
 * It stores logs in vectors by level and provides read-only accessors.
 */

#pragma once

#include "utils/ILogger.hpp"
#include <string>
#include <vector>
#include <mutex>

namespace qga::utils {

/**
 * @class MockLogger
 * @brief Captures log messages for testing purposes.
 *
 * Allows inspection of logged messages at each severity level.
 * Thread-safe, suitable for testing multithreaded components with logging.
 */
class MockLogger : public ILogger {
public:
    /// @brief Default constructor.
    MockLogger() = default;

    /// @brief Destructor.
    ~MockLogger() override = default;

    /// @brief Sets the minimum level for which logs are stored.
    void setLevel(qga::LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        level_ = level;
    }

    /// @brief Get all captured messages regardless of severity.
    std::vector<std::string> allLogs() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return logs_;
    }

    /// @brief Get captured logs by severity.
    std::vector<std::string> getLogsByLevel(qga::LogLevel level) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::string> result;
        for (const auto& [lvl, msg] : logs_by_level_) {
            if (lvl == level) {
                result.push_back(msg);
            }
        }
        return result;
    }

private:
    mutable std::mutex mutex_;
    qga::LogLevel level_ = qga::LogLevel::Debug;

    std::vector<std::string> logs_;
    std::vector<std::pair<qga::LogLevel, std::string>> logs_by_level_;

    void log(qga::LogLevel level, const std::string& msg) override{
        if (static_cast<int>(level) < static_cast<int>(level_))
            return;

        std::lock_guard<std::mutex> lock(mutex_);
        logs_.emplace_back(msg);
        logs_by_level_.emplace_back(level, msg);
    }
};


} // namespace qga::utils
