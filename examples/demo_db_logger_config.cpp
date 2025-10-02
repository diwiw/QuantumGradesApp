#include "Config.hpp"
#include "utils/SpdLogger.hpp"
#include "ingest/DataIngest.hpp"
#include "Version.hpp"

#include <iostream>
#include <memory>

using namespace qga;
using namespace utils;

int main() {
    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantumGradesApp Backtest\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    // ===== 1. Load Config =====
    auto& config = Config::getInstance();
    config.loadFromEnv();

    auto log_level = config.logLevel();

    // ===== 2. Setup Logger =====
    auto logger = std::make_shared<SpdLogger>(
        "DemoLogger",
        std::vector<std::shared_ptr<spdlog::sinks::sink>>{
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
        },
        true // async
    );
    logger->setLevel(log_level);
    logger->info("Logger initialized with level {}", toString(log_level));

    // ===== 3. Ingest Data =====
    qga::ingest::DataIngest ingest(logger);

    auto series = ingest.fromCsv("data/missing.csv"); // deliberately missing
    if (!series.has_value()) {
        logger->error("Failed to ingest data from CSV");
    } else {
        logger->info("Ingested {} bars", series->size());
    }

    logger->info("Demo finished.");
    return 0;
}
