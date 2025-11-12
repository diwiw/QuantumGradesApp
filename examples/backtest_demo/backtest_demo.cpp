#include "Version.hpp"
#include "core/Config.hpp"
#include "io/FileManager.hpp"
#include "utils/ILogger.hpp"
#include "utils/LoggerFactory.hpp"
#include <filesystem>
#include <iostream>

#include "domain/backtest/BarSeries.hpp"
#include "domain/backtest/Engine.hpp"
#include "domain/backtest/Execution.hpp"
#include "strategy/BuyHold.hpp"
// #include "strategy/MACrossover.hpp" // Optional alt strategy

int main()
{
    using namespace qga;

    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantumGradesApp Backtest Demo\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    // === Setup Logger ===
    auto logger = utils::LoggerFactory::createLogger("BacktestDemo", "logs/backtest_demo.log",
                                                     LogLevel::Info);
    logger->log(LogLevel::Info, "[APP] Backtest demo started");

    // === Load Config ===
    auto& cfg = qga::core::Config::getInstance();
    std::vector<std::string> warnings;
    cfg.loadDefaults();
    cfg.loadFromFile("config/config.json", &warnings);
    cfg.loadFromEnv(&warnings);
    for (const auto& w : warnings)
        logger->log(LogLevel::Warn, "[Config] " + w);

    // === Build BarSeries (simple 4-bar series) ===
    domain::backtest::BarSeries series;
    int64_t ts = 0;

    series.add({ts, 100.0, 100.0, 100.0, 100.0, 0.0});
    ts += 60'000;
    series.add({ts, 105.0, 105.0, 105.0, 105.0, 0.0});
    ts += 60'000;
    series.add({ts, 110.0, 110.0, 110.0, 110.0, 0.0});
    ts += 60'000;
    series.add({ts, 108.0, 108.0, 108.0, 108.0, 0.0});

    logger->log(LogLevel::Info, "[Data] Created 4-bar test series");

    // === Execution parameters ===
    domain::backtest::ExecParams exec{};
    exec.commission_fixed_ = 0.5; // 50 cents per trade
    exec.commission_bps_ = 5.0;   // 5 bps
    exec.slippage_bps_ = 10.0;    // 10 bps

    domain::backtest::Engine engine(10'000.0, exec);
    strategy::BuyHold strategy;

    logger->log(LogLevel::Info, "[Engine] Running Buy&Hold backtest");

    // === Run the backtest ===
    auto result = engine.run(series, strategy);

    // === Report ===
    std::cout << "\n=== Backtest Results ===\n";
    std::cout << "Initial equity:  " << result.initial_equity_ << "\n";
    std::cout << "Final equity:    " << result.final_equity_ << "\n";
    std::cout << "Trades executed: " << result.trades_executed_ << "\n";

    logger->log(LogLevel::Info,
                fmt::format("[Results] Initial: {:.2f}, Final: {:.2f}, Trades: {}",
                            result.initial_equity_, result.final_equity_, result.trades_executed_));

    logger->log(LogLevel::Info, "[APP] Backtest demo finished successfully.");
    return 0;
}
