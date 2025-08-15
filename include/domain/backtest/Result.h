#pragma once

namespace backtest {
    struct BacktestResult {
        double initial_equity = 10000.0;  ///< Initial equity for the backtest.
        double final_equity = 0.0;         ///< Final equity after the backtest
        int trades_executed = 0;        ///< Number of trades executed during the backtest.
    };

}   // namespace backtest