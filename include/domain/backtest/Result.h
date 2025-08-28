/**
 * @file Result.h
 * @brief Backtest outcome (summary metrics).
 */
#pragma once

namespace backtest {
    /**
     * @struct BacktestResult
     * @brief Minimal backtest summary returned by the engine.
     *
     * Extend with PnL series, drawdown, Sharpe, etc. in future versions.
     */
    struct BacktestResult {
        double initial_equity_ = 10000.0;    ///< Initial equity for the backtest.
        double final_equity_ = 0.0;          ///< Final equity after the backtest
        int trades_executed_ = 0;            ///< Number of trades executed during the backtest.
    };

}   // namespace backtest