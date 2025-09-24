/**
 * @file Engine.hpp
 * @brief Orchestrates a backtest: streams bars to a strategy and executes signals.
 *
 * Defines the main simulation loop, which runs a given strategy over a time series
 * of market data (BarSeries), applying execution logic and recording results.
 */

#pragma once

#include "domain/backtest/BarSeries.hpp"
#include "domain/backtest/Result.hpp"
#include "strategy/IStrategy.hpp"
#include "domain/backtest/Execution.hpp"

namespace qga::domain::backtest {

/**
 * @class Engine
 * @brief Runs a strategy over a series of bars and produces a result.
 *
 * Core simulation engine that performs the following steps:
 * - Initializes strategy and portfolio.
 * - Feeds market data (bars) to the strategy.
 * - Executes generated signals via slippage and commission model.
 * - Records trades and final metrics.
 */
class Engine {
    public:
        /**
         * @brief Constructs the backtest engine.
         * @param initial_equity Starting capital used in the simulation.
         * @param exec Execution parameters (slippage, commission).
         */
        explicit Engine(double initial_equity = 10000.0,
                ExecParams exec = {})
        : initial_equity_(initial_equity), exec_(exec) {}

        /**
         * @brief Execute the backtest over the given series with the provided strategy.
         * @param series Input time series of bars (OHLCV).
         * @param strat  Strategy to be executed.
         * @return BacktestResult summary (equity, trades).
         */
        BacktestResult run(BarSeries const& series, strategy::IStrategy& strat);

    private:
        double initial_equity_;  ///< Initial equity for the backtest.
        ExecParams exec_;          ///< Execution model (commissions, slippage).
};

} // namespace qga::domain::backtest
