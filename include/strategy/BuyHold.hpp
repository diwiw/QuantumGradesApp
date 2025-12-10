/**
 * @file BuyHold.hpp
 * @brief Baseline strategy: buy once at the beginning and hold.
 */
#pragma once
#include "strategy/IStrategy.hpp"

namespace qga::strategy {

/**
 * @class BuyHold
 * @brief Emits a single Buy signal on the first bar; otherwise None.
 *
 * A simple benchmark strategy that performs a single buy on the first
 * available quote and then holds the position until the end.
 *
 * Implements the @ref IStrategy interface.
 */
class BuyHold final: public IStrategy {
public:
    /**
     * @brief Called once before the backtest begins.
     */
    void onStart() override;

    /**
     * @brief Called for each quote in the time series.
     *
     * Emits a Buy signal only on the first bar; then returns None.
     *
     * @param q Market quote for the current bar.
     * @return Buy signal on first bar; None thereafter.
     */
    Signal onBar(const domain::Quote& q) override;

    /**
     * @brief Called once after the backtest ends.
     */
    void onFinish() override;

    private:
        bool has_bought_ = false;  ///< Flag to track if a buy has been made.
};

} // namespace qga::strategy
