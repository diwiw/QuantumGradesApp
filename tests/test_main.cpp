#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Logger.hpp"
#include <filesystem>
#include "Version.hpp"

struct LogInit {
	LogInit() {
		if (!std::filesystem::exists("logs"))
			std::filesystem::create_directory("logs");
		Logger::getInstance().setLogFile("logs/test_log.txt");
	}
};

LogInit log_init; 	// global object, it executed before tests
	
struct VersionPrinter {
	VersionPrinter() {
		std::cout << "=========================\n";
		std::cout << " QuantumGradesApp Tests\n";
		std::cout << " Version: " << APP_VERSION << "\n";
		std::cout << " Build date: " << APP_BUILD_DATE << "\n";
		std::cout << "=========================\n\n";
	}
};

static VersionPrinter version_printer;