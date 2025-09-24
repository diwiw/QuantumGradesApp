/**
* @file ILogger.hpp
* @brief Abstract interface for logging used across the application.
*
* Provides a clean and extensible logging interface that allows injection of concrete
* logging implementations (e.g., spdlog, mock loggers for testing).
*
* Supports structured logging with severity levels and convenient methods for each level.
*/

#pragma once

#include <string>

namespace qga::utils {

/**
* @enum LogLevel
* @brief Enumeration for severity levels used in logging.
*/
enum class LogLevel {
    Trace,    ///< Trace level for very detailed logs
    Debug,    ///< Debug level for debugging information
    Info,     ///< Info level for general information
    Warn,     ///< Warning level for potential issues
    Error,    ///< Error level for errors that need attention
    Critical  ///< Critical level for severe issues
};

/**
* @class ILogger
* @brief Abstract logger interface.
*
* Provides pure virtual methods for different log levels.
* This interface allows for dependency injection and unit testing.
*/
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
    * @brief General logging method.
    * @param level Severity level of the log message.
    * @param message Message to log.
    */
    virtual void log(LogLevel level, const std::string& message) = 0;

    /** @name Convenience Logging Methods */
    ///@{


    /** @brief Log a trace-level message. */
    virtual void trace(const std::string& message) { log(LogLevel::Trace, message); }

    /** @brief Log a debug-level message. */
    virtual void debug(const std::string& message) { log(LogLevel::Debug, message); }


    /** @brief Log an info-level message. */
    virtual void info(const std::string& message) { log(LogLevel::Info, message); }


    /** @brief Log a warning-level message. */
    virtual void warn(const std::string& message) { log(LogLevel::Warn, message); }


    /** @brief Log an error-level message. */
    virtual void error(const std::string& message) { log(LogLevel::Error, message); }


    /** @brief Log a critical-level message. */
    virtual void critical(const std::string& message) { log(LogLevel::Critical, message); }

    ///@}
};

} // namespace qga::utils
