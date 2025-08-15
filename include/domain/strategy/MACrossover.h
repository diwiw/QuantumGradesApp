#pragma once
#include <deque>
#include "strategy/IStrategy.h"

namespace strategy {

    class MACrossover final: public IStrategy {
    public:
        MACrossover(int fast=10, int slow=20);
        void onStart() override;
        Signal on_bar(const domain::Quote& q) override;
        void on_finish() override;
    
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

            