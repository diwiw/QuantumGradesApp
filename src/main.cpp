#include <iostream>
#include <filesystem>
#include "Grades.h"
#include "Version.h"
#include "FileManager.h"
#include "Logger.h"
#include "Config.h"

#include "domain/backtest/Engine.h"
#include "domain/backtest/Execution.h"
#include "domain/backtest/BarSeries.h"
#include "strategy/BuyHold.h"


int main() {
	// === Header ===
	std::cout << "===================================\n";
	std::cout << " QuantumGradesApp\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";

	Logger::getInstance().log(LogLevel::INFO, "[APP] Application started - version " + std::string(APP_VERSION));
	
	// === Build a simple time series of quotes (BarSeries)
	backtest::BarSeries series;
	int64_t ts = 0; // np. 0, albo realny epoch millis

	series.add(domain::Quote{ts, 100.0, 100.0, 100.0, 100.0, 0.0}); ts += 60'000;
	series.add(domain::Quote{ts, 105.0, 105.0, 105.0, 105.0, 0.0}); ts += 60'000;
	series.add(domain::Quote{ts, 110.0, 110.0, 110.0, 110.0, 0.0}); ts += 60'000;
	series.add(domain::Quote{ts, 108.0, 108.0, 108.0, 108.0, 0.0}); ts += 60'000;
	
	// === Strategies simple example ===
	backtest::ExecParams exec{};
	exec.commission_fixed	= 0.5;	// 50 cents per trade
	exec.commission_bps		= 5.0;	// 5bps from value additionally
	exec.slippage_bps		= 10.0;	// 10 bps -> buy more expensive, sell cheaper
	
	backtest::Engine eng(10000.0, exec);

	// === Choose a trading strategy
	strategy::BuyHold bh;						// simple "buy and hold" strategy
	// MACrossover ma{3, 5}; 		// alternative: moving-aerage crossover

	auto result = eng.run(series, bh);

	// === Report results
	std::cout << "Initial equity: " << result.initial_equity << "\n";
	std::cout << "Final equity: " << result.final_equity << "\n";
	std::cout << "Trades executed: " << result.trades_executed << "\n\n";


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
