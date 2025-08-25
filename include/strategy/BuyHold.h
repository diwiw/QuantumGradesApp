/**
 * @file BuyHold.h
 * @brief Baseline strategy: buy once at the beginning and hold.
 */
#pragma once
#include "strategy/IStrategy.h"

namespace strategy {
/**
 * @class BuyHold
 * @brief Emits a single Buy signal on the first bar; otherwise None.
 *
 * Useful as a benchmark against more complex strategies.
 */
    class BuyHold final: public IStrategy {
    public:
        void onStart() override;    

        Signal onBar(const domain::Quote& q) override;

        void onFinish() override;
        
        private:
            bool has_bought_ = false;  ///< Flag to track if a buy has been made.
    };
}   // namespace strategy