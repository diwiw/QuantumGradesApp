#include "doctest.h"
#include "Logger.h"
#include <fstream>
#include <string>

TEST_CASE("Logger logs to file") {
	const std::string logFilePath = "logs/log.txt";
	Logger& logger = Logger::getInstance();

	// Clearing the file before test
	std::ofstream clearFile(logFilePath, std::ios::trunc);
	clearFile.close();

	// Logging sample message
	logger.log(LogLevel::INFO, "Test log message");

	// Checking, if the message was saved
	std::ifstream logFile(logFilePath);
	std::string line;
	bool found = false;

	while (std::getline(logFile, line)) {
		if(line.find("Test log message") != std::string::npos) {
			found = true;
			break;
		}
	}
	CHECK(found);
}
