/**
 * @file IStrategy.hpp
 * @brief Strategy interface returning trading signals per bar.
 */
#pragma once
#include "domain/Quote.hpp"

namespace qga::strategy {

/**
 * @enum Signal
 * @brief Possible trading decisions a strategy can emit at a bar.
 *
 * - None: No action taken.
 * - Buy: Enter or increase a long position.
 * - Sell: Exit or reduce a long position.
 */
enum class Signal { None, Buy, Sell };

/**
 * @class IStrategy
 * @brief Contract for trading strategies used by the backtest engine.
 *
 * This abstract interface must be implemented by any trading strategy class.
 * The backtest engine invokes the following lifecycle methods:
 *
 * - @ref onStart()   → called once before the first bar.
 * - @ref onBar(q)    → called for each bar, returns @ref Signal.
 * - @ref onFinish()  → called once after the final bar.
 *
 * Strategies must be stateless across multiple backtests or reset properly.
 */
class IStrategy {
public:
    /**
    * @brief Virtual destructor.
    */
    virtual ~IStrategy() = default;

    /**
     * @brief Prepare internal state. Called once before streaming bars.
     */
    virtual void onStart() {}

    /**
     * @brief Consume next market bar and emit a trading decision.
     *
     * @param q Incoming market quote (bar).
     * @return A @ref Signal (Buy, Sell, or None).
     */
    virtual Signal onBar(const domain::Quote& q) = 0;

    /**
     * @brief Cleanup or finalize strategy state. Called after the last bar.
     */
    virtual void onFinish() {}
};

} // namespace qga::strategy
