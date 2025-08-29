#include "doctest.h"
#include "Logger.h"
#include <fstream>
#include <string>

TEST_CASE("Logger basic log to file") {
	const std::string TEST_LOG_FILE = "log_test.txt";
	//Logger& logger = Logger::getInstance();
	std::remove(TEST_LOG_FILE.c_str());
	Logger::getInstance().setLogFile(TEST_LOG_FILE);
	
	// Clearing the file before test
	std::string msg = "Test log message";
	Logger::getInstance().log(LogLevel::INFO, msg);
		
	// Checking, if the message was saved
	std::ifstream log_file(TEST_LOG_FILE);
	REQUIRE(log_file.is_open());

	std::string line;
	bool found = false;

	while (std::getline(log_file, line)) {
		if(line.find(msg) != std::string::npos) {
			found = true;
			break;
		}
	}
	CHECK(found);

	log_file.close();
	std::remove(TEST_LOG_FILE.c_str());
}
