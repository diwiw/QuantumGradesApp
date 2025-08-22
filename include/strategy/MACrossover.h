/**
 * @file MACrossover.h
 * @brief Simple SMA fast/slow crossover strategy.
 */
#pragma once
#include <deque>
#include "strategy/IStrategy.h"

namespace strategy {
/**
 * @class MACrossover
 * @brief Emits Buy when SMA(fast) crosses above SMA(slow), Sell on reverse.
 *
 * Rolling windows are maintained internally to compute SMAs.
 */
    class MACrossover final: public IStrategy {
    public:
        /// Construct with window sizes (fast < slow recommended).
        MACrossover(int fast_period=10, int slow_period=20);
        void onStart() override;
        Signal onBar(const domain::Quote& q) override;
        void onFinish() override;
    
    private:
        int fast_period_;  ///< Fast moving average period.
        int slow_period_;  ///< Slow moving average period.
        std::deque<double> w_fast_;  ///< Fast rolling window of closes.
        std::deque<double> w_slow_;  ///< Slow rolling window of closes.
        double sum_fast_ = 0.0, sum_slow_ = 0.0;  ///< Sums for fast and slow moving averages.
        double prev_fast_ = 0.0, prev_slow_ = 0.0;
        bool ready_ = false;  ///< Flag to indicate if enough data is available for moving averages.
    };
} // namespace strategy

            