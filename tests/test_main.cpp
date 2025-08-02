#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Logger.h"
#include <filesystem>

struct LogInit {
	LogInit() {
		if (!std::filesystem::exists("logs"))
			std::filesystem::create_directory("logs");
		Logger::getInstance().setLogFile("logs/test_log.txt");
	}
};

LogInit log_init; 	// global object, it executed before tests
