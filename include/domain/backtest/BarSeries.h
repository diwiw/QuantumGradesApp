/**
 * @file BarSeries.h
 * @brief Container of market bars used by the backtest engine.
 */
#pragma once
#include <vector>
#include "domain/Quote.h"

namespace backtest {
/**
 * @class BarSeries
 * @brief Simple time-ordered container for @ref domain::Quote bars.
 *
 * Provides read-only accessors for the engine and strategies.
 */
    class BarSeries {
    public:
        using Quote = domain::Quote;
        /// Append a new bar to the series.
        void add(const domain::Quote& q);
        
        /// Number of bars.
        std::size_t size() const noexcept;
        /// Bounds-checked access to i-th bar.
        const Quote& at(std::size_t i) const;
        /// Reference to the last bar (precondition: size() > 0).
        const Quote& back() const;
    private:
        std::vector<Quote> data_;
    };
} // namespace backtest