/**
 * @file SpdLogger.hpp
 * @brief Asynchronous logger implementation using spdlog.
 */

#pragma once

#include "utils/ILogger.hpp"
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace utils {

    /**
     * @class SpdLogger
     * @brief Asynchronous, thread-safe logger using spdlog library.
     *
     * This logger supports both console and file output,
     * runs in a dedicated logging thread, and formats log messages with timestamps and severity levels.
     */
    class SpdLogger : public ILogger {
    public:
        /**
         * @brief Constructor initializing async spdlog logger.
         * @param logger_name Logical name for the logger instance.
         * @param file_path Path to the log file.
         * @param level Minimum severity level to log (default: LogLevel::Info).
         */
        SpdLogger(const std::string& logger_name,
                  const std::string& file_path,
                  LogLevel level = LogLevel::Info);
        
        /**
         * @brief Construct logger with custom sinks (e.g. console, rotating file).
         * @param logger_name Logical name for the logger instance.
         * @param sinks Vector of spdlog sink pointers for output destinations.
         * @param async_mode If true, uses asynchronous logging (default: false).
         */
        SpdLogger(const std::string& logger_name,
          const std::vector<std::shared_ptr<spdlog::sinks::sink>>& sinks,
          bool async_mode = false);

        /**
         * @brief Logs a message at a specific log level.
         * @param level LogLevel to use (Trace, Debug, Info, etc.).
         * @param message Message string to log.
         */
        void log(LogLevel level, const std::string& message) override;


        /**
         * @brief Converts LogLevel enum to spdlog level enum.
         * @param level LogLevel value.
         * @return Corresponding spdlog::level::level_enum value.
         */
        spdlog::level::level_enum setLevel(LogLevel level);

    private:
        std::shared_ptr<spdlog::logger> spd_logger_;  ///< Underlying spdlog instance.
        static spdlog::level::level_enum toSpdLevel(LogLevel level);  ///< Converts LogLevel to spdlog level.
    };

}   // namespace utils