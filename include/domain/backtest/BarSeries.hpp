/**
 * @file BarSeries.hpp
 * @brief Container of market bars used by the backtest engine.
 *
 * Defines a simple container for storing and accessing time-ordered
 * market quotes (OHLCV data). Used internally by the engine and strategies.
 */

#pragma once

#include <vector>
#include "domain/Quote.hpp"

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
         * @brief Provides direct (unchecked) access to a quote by index.
         *
         * @param i Index into the series.
         * @return Const reference to the quote at the specified index.
         *
         * @note Unlike `at()`, this method does not perform bounds checking.
         *       Use only when index safety is guaranteed.
         */
        const Quote& operator[](std::size_t i) const;

        /**
         * @brief Returns the most recent bar in the series.
         * @return Const reference to the last quote.
         * @pre `size() > 0`
         */
        const Quote& end() const;

        /**
         * @brief Returns the first bar in the series.
         * @return Const reference to the first quote.
         * @pre `size() > 0`
         */
        const Quote& front() const;

       /**
         * @brief Checks if the BarSeries contains any quotes.
         *
         * This method allows users to verify whether the series is empty,
         * which can be useful for validating data loading operations or 
         * ensuring proper initialization before processing.
         *
         * @return True if the BarSeries contains no quotes, false otherwise.
         *
         * @note This method is noexcept and does not modify the state of the object.
         */
        bool empty() const noexcept;

        /**
         * @brief Clears all stored quotes from the series.
         *
         * Resets the container to an empty state. Useful when reloading
         * or reusing the series instance.
         */
        void clear() noexcept;

        /**
         * @brief Provides read-only access to the internal container of quotes.
         *
         * This method enables external components (e.g. exporters or visualizers)
         * to iterate over the entire series without exposing mutation capabilities.
         *
         * @return Const reference to the internal std::vector of Quote objects.
         *
         * @note Usage of this method preserves encapsulation while enabling iteration.
         */
        const std::vector<Quote>& data() const noexcept; 
    private:
        std::vector<Quote> data_;   ///< Internal container of quotes.
    };

} // namespace backtest