/**
 * @file Logger.hpp
 * @brief Thread-safe, singleton logger class with log-level filtering and file output.
 */

#pragma once

#include "core/Platform.hpp" // Ensure platform-specific settings are included first
#include "common/LogLevel.hpp"
#include <fstream>
#include <string>
#include <memory>
#include <mutex>

namespace qga::utils {

/**
 * @class Logger
 * @brief Singleton logger class for structured and thread-safe log output.
 *
 * Logger allows writing messages with severity leveles to a log file and optionally stdout.
 * It uses the Singleton pattern to ensure a single instance across the application.
 */
class Logger {
public:
	/**
	 * @brief Get the singleton instance of th eLogger
	 * @return Reference to the Logger instance
	 */
	static Logger& getInstance();

	/**
	 * @brief Set the minimum log level for output
	 * @param level LogLevel (e.g., INFO, ERROR)
	 */
	void setLogLevel(qga::LogLevel level);

	/**
	 * @brief Set the log output file
	 * @param filename Path to the file
	 */
	void setLogFile(const std::string& filename);

	/**
	 * @brief Log a message with a given severity level
	 * @param level LogLevel
	 * @param message Message string to log
	 */
	void log(qga::LogLevel level, const std::string& message);

private:
	Logger();	///< Private constructor (singleton pattern)
	~Logger();	///< Destructor to close log file
	Logger(const Logger&) = delete;	///< Deleted assignment
	Logger& operator=(const Logger&) = delete; ///< Deleted assignment
	std::string levelToString(qga::LogLevel level); ///< Convert level to string
	std::string getCurrentTime();		   ///< Get current timestamp

	std::ofstream logfile_;			   ///< Log file stream
	qga::LogLevel current_level_;			   ///< Currenet minimum log level
	std::mutex mtx_;				   ///< Thread safety mutex

};

} // namespace qga::utils
