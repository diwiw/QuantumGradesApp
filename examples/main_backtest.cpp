#include <iostream>
#include <filesystem>
#include "Grades.hpp"
#include "Version.hpp"
#include "FileManager.hpp"
#include "utils/Logger.hpp"
#include "Config.hpp"

#include "domain/backtest/Engine.hpp"
#include "domain/backtest/Execution.hpp"
#include "domain/backtest/BarSeries.hpp"
#include "strategy/BuyHold.hpp"


int main() {
	// === Header ===
	std::cout << "===================================\n";
	std::cout << " QuantumGradesApp Backtest\n";
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
	exec.commission_fixed_	= 0.5;	// 50 cents per trade
	exec.commission_bps_		= 5.0;	// 5bps from value additionally
	exec.slippage_bps_		= 10.0;	// 10 bps -> buy more expensive, sell cheaper

	backtest::Engine eng(10000.0, exec);

	// === Choose a trading strategy
	strategy::BuyHold bh;						// simple "buy and hold" strategy
	// MACrossover ma{3, 5}; 		// alternative: moving-aerage crossover

	auto result = eng.run(series, bh);

	// === Report results
	std::cout << "Initial equity: " << result.initial_equity_ << "\n";
	std::cout << "Final equity: " << result.final_equity_ << "\n";
	std::cout << "Trades executed: " << result.trades_executed_ << "\n\n";

	return 0;

}