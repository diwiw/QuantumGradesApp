#include "Grades.h"
#include "Version.h"
#include "FileManager.h"
#include "Logger.h"
#include <iostream>

int main() {
	// === Header ===
	std::cout << "===================================\n";
	std::cout << " QuantumGradesApp\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";

	Logger::getInstance().log(LogLevel::INFO, "[APP] Application started - version " + std::string(APP_VERSION));
	
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
	std::cout << "\n Saving grades to \"data/grades.txt\"...\n";
	std::vector<std::string> gradeLines;
	for (int note : g.getNotes()) {
		gradeLines.push_back(std::to_string(note));
	}

	if (FileManager::writeAllLines("data/grades.txt", gradeLines)) {
		Logger::getInstance().log(LogLevel::INFO, "[APP] Grades saved to data/grades.txt");
		std::cout << "Grades saved successfully\n";
	} else {
		Logger::getInstance().log(LogLevel::ERROR, "[APP] Failed to save grade to file");
		std::cerr << "Error: Could not save grades to file.\n";
	}
	std::cout << "Application reads data from a file and print them plus calculates and presents statistics\n";
	Logger::getInstance().log(LogLevel::INFO, "[APP] Application reads data from a file (data/readGrades.txt) presents them and their statistics");
	
	// === Read external file and print ===
	const std::string filePath = "data/readGrades.txt";
	std::cout << "\nReading data from: " << filePath << "\n";
	Logger::getInstance().log(LogLevel::INFO, "[APP] Reading data from " + filePath);
	
	auto linesOpt = FileManager::readAllLines(filePath);
	if(!linesOpt.has_value()) {
		Logger::getInstance().log(LogLevel::ERROR, "[APP] Failed to read file: " + filePath);
		std::cerr << "Error: Could not read file: " << filePath << "\n";
		return 1;
	}

	const auto& lines = linesOpt.value();
	if (lines.empty()) {
		Logger::getInstance().log(LogLevel::WARNING, "[APP] File is empty: " + filePath);
		std::cout << "No data found in file: " << filePath << "\n";
		return 0;
	}

	// === Display the file ===
	std::cout << "=== File content: " << filePath << " ===\n";
	for (const auto& line : lines) {
		std::cout << line << "\n";
	}

	Logger::getInstance().log(LogLevel::INFO, "[APP] File read successfully: " + filePath);
	Logger::getInstance().log(LogLevel::INFO, "[APP] Application finished");	

	return 0;
	
}
