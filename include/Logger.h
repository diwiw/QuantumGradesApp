// Logger.h
#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <mutex>

/// Enum class representing severity levels for log messages
enum class LogLevel {
	DEBUG,		///< Detailed debug info
	INFO,		///< General information
	WARNING,	///< Warings about potential issues
	ERROR,		///< Errors
	CRITICAL	///< Critical Errors
};

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
	void setLogLevel(LogLevel level);

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
	void log(LogLevel level, const std::string& message);

private:
	Logger();	///< Private constructor (singleton pattern)
	~Logger();	///< Destructor to close log file
	Logger(const Logger&) = delete;	///< Deleted assignment 
	Logger& operator=(const Logger&) = delete; ///< Deleted assignment
	std::string levelToString(LogLevel level); ///< Convert level to string
	std::string getCurrentTime();		   ///< Get current timestamp
						   
	std::ofstream logfile;			   ///< Log file stream
	LogLevel currentLevel;			   ///< Currenet minimum log level
	std::mutex mtx;				   ///< Thread safety mutex

};
