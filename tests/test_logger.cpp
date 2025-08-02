#include "doctest.h"
#include "Logger.h"
#include <fstream>
#include <string>

TEST_CASE("Logger basic log to file") {
	const std::string testLogFile = "bin/log_test.txt";
	//Logger& logger = Logger::getInstance();
	std::remove(testLogFile.c_str());
	Logger::getInstance().setLogFile(testLogFile);
	
	// Clearing the file before test
	std::string msg = "Test log message";
	Logger::getInstance().log(LogLevel::INFO, msg);
		
	// Checking, if the message was saved
	std::ifstream logFile(testLogFile);
	REQUIRE(logFile.is_open());

	std::string line;
	bool found = false;

	while (std::getline(logFile, line)) {
		if(line.find(msg) != std::string::npos) {
			found = true;
			break;
		}
	}
	CHECK(found);

	logFile.close();
	std::remove(testLogFile.c_str());
}
