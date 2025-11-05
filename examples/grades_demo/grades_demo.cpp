#include <iostream>
#include <filesystem>
#include "Grades.hpp"
#include "Version.hpp"
#include "io/FileManager.hpp"
#include "utils/Logger.hpp"
#include "Config.hpp"

int main() {
	// === Header ===
	std::cout << "===================================\n";
  	std::cout << " QuantumGradesApp Config_Grades\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";

	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Application started - version " + std::string(APP_VERSION));

    // === Load configuration ===
	std::cout << "Loading configuration...\n";

    
	using qga::Config;
	std::vector<std::string> warnings;
	Config& cfg = Config::getInstance();
    // Defaults -> file -> ENV (layers order)
	cfg.loadDefaults();
	cfg.loadFromFile("config/config.json", &warnings);
	cfg.loadFromEnv(&warnings);
    for(const auto& w : warnings)
		std::cerr << "[config] " << w << '\n';
	std::cout << "dataDir=" << cfg.dataDir().string()
		<< " threads=" << cfg.threads()
		<< " logLevel=" << qga::toString(cfg.logLevel())
		<< " logFile=" << cfg.logFile().string() << '\n';	
	
	// === Add grades ===
	qga::Grades g;
	std::cout << "Application adds notes, prints them and their statistics\n";
    g.add(5);
	g.add(4);
	g.add(3);
	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Application adds notes: 5, 4, 3, prints grades and their statistics");
	std::cout << "Added notes: 5, 4, 3\n";
	g.printGrades();
	g.printSummary();
	// === Save grades ===
	std::cout << "\nSaving grades to \"data/grades.txt\"...\n";
	std::vector<std::string> grade_lines;
	for (int note : g.getNotes()) {
		grade_lines.push_back(std::to_string(note));
	}
	if (qga::io::FileManager::writeAllLines("data/grades.txt", grade_lines)) {
		qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Grades saved to data/grades.txt");
		std::cout << "Grades saved successfully\n";
	} else {
		qga::utils::Logger::getInstance().log(qga::LogLevel::Err, "[APP] Failed to save grade to file");
		std::cerr << "Error: Could not save grades to file.\n";
	}
	std::cout << "Application reads data from a file and print them plus calculates and presents statistics\n";
	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Application reads data from a file (data/readGrades.txt) presents them and their statistics");

	// === Read external file and print ===
	const std::string FILE_PATH = "data/readGrades.txt";
	std::cout << "\nReading data from: " << FILE_PATH << "\n";
	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Reading data from " + FILE_PATH);

	auto lines_opt = qga::io::FileManager::readAllLines(FILE_PATH);
	if(!lines_opt.has_value()) {
		qga::utils::Logger::getInstance().log(qga::LogLevel::Err, "[APP] Failed to read file: " + FILE_PATH);
		std::cerr << "Error: Could not read file: " << FILE_PATH << "\n";
		return 1;
	}
	const auto& lines = lines_opt.value();
    if (lines.empty()) {
		qga::utils::Logger::getInstance().log(qga::LogLevel::Warn, "[APP] File is empty: " + FILE_PATH);
		std::cout << "No data found in file: " << FILE_PATH << "\n";
		return 0;
	}
	// === Display the file ===
	std::cout << "=== File content: " << FILE_PATH << " ===\n";
	for (const auto& line : lines) {
		std::cout << line << "\n";
	}
	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] File read successfully: " + FILE_PATH);
	qga::utils::Logger::getInstance().log(qga::LogLevel::Info, "[APP] Application finished");
	return 0;
	
}