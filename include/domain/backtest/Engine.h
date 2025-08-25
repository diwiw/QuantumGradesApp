/**
 * @file Engine.h
 * @brief Orchestrates a backtest: streams bars to a strategy and executes signals.
 */
#pragma once
#include "domain/backtest/BarSeries.h"
#include "domain/backtest/Result.h"
#include "strategy/IStrategy.h"
#include "domain/backtest/Execution.h"

namespace backtest {
    /**
     * @class Engine
     * @brief Runs a strategy over a series of bars and produces a result.
     *
     * Template method flow:
     *  - strategy.on_start();
     *  - for each bar: sig = strategy.on_bar(bar); execute(sig);
     *  - strategy.on_finish();
     */
    class Engine {
        public:
            /// @param initial_equity Starting capital used in the simulation.
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
            ExecParams exec_;
    };
}   // namespace backtest