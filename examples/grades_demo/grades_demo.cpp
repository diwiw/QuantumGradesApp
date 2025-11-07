/**
 * @file grades_demo.cpp
 * @brief Legacy demo showing basic use of Grades, FileManager, and Config
 *        with the new asynchronous logging interface.
 */

#include <iostream>
#include <filesystem>
#include "Grades.hpp"
#include "core/Version.hpp"
#include "io/FileManager.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"
#include "core/Config.hpp"

using namespace qga;

/**
 * @brief Entry point for the Grades demo.
 * 
 * Demonstrates usage of Grades, Config, and FileManager with
 * asynchronous logging via the new LoggerFactory system.
 */
int main() {
    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantumGradesApp Config_Grades\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    // === Initialize logger ===
    auto logger = qga::utils::LoggerFactory::createLogger(
        "GradesDemoApp",
        "logs/grades_demo_app.log",
        qga::LogLevel::Info
    );
    logger->log(LogLevel::Info, fmt::format("[APP] Started - version {}", APP_VERSION));

    // === Load configuration ===
    std::cout << "Loading configuration...\n";
    logger->log(LogLevel::Info, "[Config] Loading configuration...");

    using qga::Config;
    std::vector<std::string> warnings;
    Config& cfg = Config::getInstance();

    cfg.loadDefaults();
    cfg.loadFromFile("config/config.json", &warnings);
    cfg.loadFromEnv(&warnings);

    for (const auto& w : warnings)
        logger->log(LogLevel::Warn, fmt::format("[Config] {}", w));

    std::cout << "dataDir=" << cfg.dataDir().string()
              << " threads=" << cfg.threads()
              << " logLevel=" << qga::toString(cfg.logLevel())
              << " logFile=" << cfg.logFile().string() << '\n';

    // === Grades demo ===
    qga::Grades g;
    logger->log(LogLevel::Info, "[GradesDemo] Adding notes: 5, 4, 3");
    std::cout << "Adding notes: 5, 4, 3\n";
    g.add(5);
    g.add(4);
    g.add(3);

    g.printGrades();
    g.printSummary();

    // === Save grades ===
    std::cout << "\nSaving grades to \"data/grades.txt\"...\n";
    std::vector<std::string> grade_lines;
    for (int note : g.getNotes()) {
        grade_lines.push_back(std::to_string(note));
    }

    if (qga::io::FileManager::writeAllLines("data/grades.txt", grade_lines)) {
        logger->log(LogLevel::Info, "[FileManager] Grades saved to data/grades.txt");
        std::cout << "Grades saved successfully\n";
    } else {
        logger->log(LogLevel::Err, "[FileManager] Failed to save grades to file");
        std::cerr << "Error: Could not save grades to file.\n";
    }

    // === Read file ===
    const std::string FILE_PATH = "data/readGrades.txt";
    std::cout << "\nReading data from: " << FILE_PATH << "\n";
    logger->log(LogLevel::Info, fmt::format("[FileManager] Reading data from {}", FILE_PATH));

    auto lines_opt = qga::io::FileManager::readAllLines(FILE_PATH);
    if (!lines_opt.has_value()) {
        logger->log(LogLevel::Err, fmt::format("[FileManager] Failed to read file: {}", FILE_PATH));
        std::cerr << "Error: Could not read file: " << FILE_PATH << "\n";
        return 1;
    }

    const auto& lines = lines_opt.value();
    if (lines.empty()) {
        logger->log(LogLevel::Warn, fmt::format("[FileManager] File is empty: {}", FILE_PATH));
        std::cout << "No data found in file: " << FILE_PATH << "\n";
        return 0;
    }

    // === Display file ===
    std::cout << "=== File content: " << FILE_PATH << " ===\n";
    for (const auto& line : lines)
        std::cout << line << "\n";

    logger->log(LogLevel::Info, fmt::format("[FileManager] File read successfully: {}", FILE_PATH));
    logger->log(LogLevel::Info, "[APP] Application finished successfully.");

    return 0;
}
