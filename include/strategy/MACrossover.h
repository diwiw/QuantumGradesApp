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
     * @brief A simple moving average crossover strategy.
     *
     * This strategy emits:
     * - `Signal::Buy` when the fast SMA crosses above the slow SMA.
     * - `Signal::Sell` when the fast SMA crosses below the slow SMA.
     *
     * Internally maintains two rolling windows of close prices to compute the averages.
     *
     * Common use cases:
     * - `fast_period = 10`, `slow_period = 20` (default)
     */
    class MACrossover final: public IStrategy {
    public:
        /**
         * @brief Construct a new MACrossover strategy.
         * @param fast_period Period of the fast moving average.
         * @param slow_period Period of the slow moving average.
         *
         * @note fast_period should be less than slow_period for meaningful signals.
         */
        explicit MACrossover(int fast_period=10, int slow_period=20);

        /**
         * @brief Initializes internal state before streaming bars.
         */
        void onStart() override;

        /**
         * @brief Called for each bar. Emits Buy/Sell/None signals.
         * @param q Incoming market quote (bar).
         * @return Trading decision based on moving average crossover.
         */
        Signal onBar(const domain::Quote& q) override;

        /**
         * @brief Cleanup or finalize internal state after last bar.
         */
        void onFinish() override;
    
    private:
         int fast_period_;   ///< Number of bars for the fast SMA.
        int slow_period_;   ///< Number of bars for the slow SMA.

        std::deque<double> w_fast_;  ///< Rolling window of closing prices for fast SMA.
        std::deque<double> w_slow_;  ///< Rolling window of closing prices for slow SMA.

        double sum_fast_ = 0.0;      ///< Running sum for fast SMA calculation.
        double sum_slow_ = 0.0;      ///< Running sum for slow SMA calculation.

        double prev_fast_ = 0.0;     ///< Fast SMA value from previous bar.
        double prev_slow_ = 0.0;     ///< Slow SMA value from previous bar.

        bool ready_ = false;         ///< Indicates if enough data is available to compute both SMAs.
    };
} // namespace strategy

            