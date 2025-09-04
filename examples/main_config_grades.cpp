#include <iostream>
#include <filesystem>
#include "Grades.hpp"
#include "Version.hpp"
#include "FileManager.hpp"
#include "utils/Logger.hpp"
#include "Config.hpp"

int main() {
	// === Header ===
	std::cout << "===================================\n";
  std::cout << " QuantumGradesApp Config_Grades\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";

	Logger::getInstance().log(LogLevel::INFO, "[APP] Application started - version " + std::string(APP_VERSION));

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
		<< " logLevel=" << Config::toString(cfg.logLevel())
		<< " logFile=" << cfg.logFile().string() << '\n';	
	
	// === Add grades ===
	Grades g;
	std::cout << "Application adds notes, prints them and their statistics\n";
    g.add(5);
	g.add(4);
	g.add(3);
	Logger::getInstance().log(LogLevel::INFO, "[APP] Application adds notes: 5, 4, 3, prints grades and their statistics");
	std::cout << "Added notes: 5, 4, 3\n";
	g.printGrades();
	g.printSummary();
	// === Save grades ===
	std::cout << "\nSaving grades to \"data/grades.txt\"...\n";
	std::vector<std::string> grade_lines;
	for (int note : g.getNotes()) {
		grade_lines.push_back(std::to_string(note));
	}
	if (FileManager::writeAllLines("data/grades.txt", grade_lines)) {
		Logger::getInstance().log(LogLevel::INFO, "[APP] Grades saved to data/grades.txt");
		std::cout << "Grades saved successfully\n";
        } else {
		Logger::getInstance().log(LogLevel::ERROR, "[APP] Failed to save grade to file");
		std::cerr << "Error: Could not save grades to file.\n";
	}
	std::cout << "Application reads data from a file and print them plus calculates and presents statistics\n";
	Logger::getInstance().log(LogLevel::INFO, "[APP] Application reads data from a file (data/readGrades.txt) presents them and their statistics");
	
	// === Read external file and print ===
	const std::string FILE_PATH = "data/readGrades.txt";
	std::cout << "\nReading data from: " << FILE_PATH << "\n";
	Logger::getInstance().log(LogLevel::INFO, "[APP] Reading data from " + FILE_PATH);
	
	auto lines_opt = FileManager::readAllLines(FILE_PATH);
	if(!lines_opt.has_value()) {
		Logger::getInstance().log(LogLevel::ERROR, "[APP] Failed to read file: " + FILE_PATH);
		std::cerr << "Error: Could not read file: " << FILE_PATH << "\n";
		return 1;
	}
	const auto& lines = lines_opt.value();
    if (lines.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[APP] File is empty: " + FILE_PATH);
		std::cout << "No data found in file: " << FILE_PATH << "\n";
		return 0;
	}
	// === Display the file ===
	std::cout << "=== File content: " << FILE_PATH << " ===\n";
	for (const auto& line : lines) {
		std::cout << line << "\n";
	}
	Logger::getInstance().log(LogLevel::INFO, "[APP] File read successfully: " + FILE_PATH);
	Logger::getInstance().log(LogLevel::INFO, "[APP] Application finished");	
	return 0;
	
}