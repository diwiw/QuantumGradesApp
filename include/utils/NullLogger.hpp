/**
 * @file NullLogger.hpp
 * @brief Implementation of ILogger that discards all log messages (Null Object Pattern).
 *
 * This logger is useful in contexts where logging is optional or should be disabled.
 * It conforms to the ILogger interface but performs no actions.
 */

#pragma once

#include "utils/ILogger.hpp"

namespace utils {

    /**
     * @class NullLogger
     * @brief A no-op logger implementing the ILogger interface.
     *
     * Follows the Null Object Pattern. Can be injected anywhere an ILogger is required,
     * but will silently discard all log calls.
     */
    class NullLogger : public ILogger {
    public:
        /// @brief Default constructor.
        NullLogger() = default;

        /// @brief Destructor.
        ~NullLogger() override = default;

        /// @brief Does nothing.
        void debug(const std::string& /*msg*/) override {}

        /// @brief Does nothing.
        void info(const std::string& /*msg*/) override {}

        /// @brief Does nothing.
        void warn(const std::string& /*msg*/) override {}

        /// @brief Does nothing.
        void error(const std::string& /*msg*/) override {}

        /// @brief Does nothing.
        void critical(const std::string& /*msg*/) override {}

        /// @brief Does nothing.
        void setLevel(LogLevel /*level*/) {}

        /// @brief Does nothing.
        void log(LogLevel /*level*/, const std::string& /*msg*/)  override{}

    };

} // namespace utils
