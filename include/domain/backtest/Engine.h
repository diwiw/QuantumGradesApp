#pragma once
#include "backtest/BarSeries."
#include "backtest/Result.h"
#include "strategy/IStrategy.h"

namespace backtest {
    class Engine {
        public:
            explicit Engine(double initial_equity = 10000.0)
            :            initial_equity_(initial_equity) {}

            BacktestResult run(BarSeries const& series, strategy::IStrategy& strat);

        private:
            double initial_equity_;  ///< Initial equity for the backtest.
    };
}   // namespace backtest