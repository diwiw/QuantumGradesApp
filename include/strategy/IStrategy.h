/**
 * @file IStrategy.h
 * @brief Strategy interface returning trading signals per bar.
 */
#pragma once
#include "domain/Quote.h"

namespace strategy {
    /**
    * @enum Signal
    * @brief Possible trading decisions a strategy can emit at a bar.
    */
    enum class Signal { None, Buy, Sell };
    /**
     * @interface IStrategy
     * @brief Contract for strategies used by the backtest engine.
     *
     * Engine calls:
     *  - on_start()  once before the first bar,
     *  - on_bar(q)   for each bar, expecting a @ref Signal,
     *  - on_finish() once after the last bar.
     */
    class IStrategy {
    public:
        virtual ~IStrategy() = default;
        /// Prepare internal state. Called once before streaming bars.
        virtual void onStart() {}
        /// Consume next bar and emit trading decision.
        virtual Signal onBar(const domain::Quote& q) = 0;
        /// Cleanup/finalize internal state. Called after the last bar.
        virtual void onFinish() {}
    };

} // namespace strategy