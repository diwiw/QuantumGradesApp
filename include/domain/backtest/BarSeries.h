/**
 * @file BarSeries.h
 * @brief Container of market bars used by the backtest engine.
 *
 * Defines a simple container for storing and accessing time-ordered
 * market quotes (OHLCV data). Used internally by the engine and strategies.
 */

#pragma once

#include <vector>
#include "domain/Quote.h"

namespace backtest {

    /**
     * @class BarSeries
     * @brief Simple time-ordered container for @ref domain::Quote bars.
     *
     * Provides read-only accessors and append functionality for use in
     * backtests. Acts as the primary data source for strategy evaluation.
     */
    class BarSeries {
    public:
        using Quote = domain::Quote;
        /**
        * @brief Appends a new market bar (quote) to the series.
        * @param q Market quote containing OHLCV data and timestamp.
        */
        void add(const domain::Quote& q);
        
        /**
         * @brief Returns the number of bars in the series.
         * @return Total number of quotes stored.
         */
        std::size_t size() const noexcept;

        /**
         * @brief Returns a const reference to the i-th bar.
         * @param i Index into the series.
         * @return Const reference to the requested quote.
         * @throws std::out_of_range if index is invalid.
         */
        const Quote& at(std::size_t i) const;

        /**
         * @brief Returns the most recent bar in the series.
         * @return Const reference to the last quote.
         * @pre `size() > 0`
         */
        const Quote& back() const;
    private:
        std::vector<Quote> data_;   ///< Internal container of quotes.
    };
} // namespace backtest