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
#include <utility>
#include <fmt/core.h>
#include "common/LogLevel.hpp"

namespace qga::utils {

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
    virtual void log(qga::LogLevel level, const std::string& message) = 0;

    /** @name Convenience Logging Methods */
    ///@{


    /** @brief Log a trace-level message. */
    virtual void trace(const std::string& message) { log(qga::LogLevel::Trace, message); }

    /** @brief Log a debug-level message. */
    virtual void debug(const std::string& message) { log(qga::LogLevel::Debug, message); }


    /** @brief Log an info-level message. */
    virtual void info(const std::string& message) { log(qga::LogLevel::Info, message); }


    /** @brief Log a warning-level message. */
    virtual void warn(const std::string& message) { log(qga::LogLevel::Warn, message); }


    /** @brief Log an error-level message. */
    virtual void error(const std::string& message) { log(qga::LogLevel::Err, message); }


    /** @brief Log a critical-level message. */
    virtual void critical(const std::string& message) { log(qga::LogLevel::Critical, message); }

    ///@}

     /** @name Convenience Logging Methods with fmt formatting */
    ///@{
    template<typename... Args>
    void trace(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Trace, fmt::format(message, std::forward<Args>(args)...)); 
    }

    template<typename... Args>
    void debug(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Debug, fmt::format(message, std::forward<Args>(args)...)); 
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Info, fmt::format(message, std::forward<Args>(args)...)); 
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Warn, fmt::format(message, std::forward<Args>(args)...)); 
    }

    template<typename... Args>
    void error(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Err, fmt::format(message, std::forward<Args>(args)...)); 
    }

    template<typename... Args>
    void critical(fmt::format_string<Args...> message, Args&&... args) { 
        log(qga::LogLevel::Critical, fmt::format(message, std::forward<Args>(args)...)); 
    }
    ///@}
};

} // namespace qga::utils
