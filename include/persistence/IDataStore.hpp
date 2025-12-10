/** @file IDataStore.hpp
 *  @brief Abstract interface for persistence layer (quotes, bar series, portfolios, trades).
 *
 * Provides unified access to underlying storage (SQLite, DuckDB, Postgres...).
 * Each implementation must ensure thread safety if used concurrently.
 */

#pragma once

#include "domain/Quote.hpp"
#include "domain/backtest/BarSeries.hpp"
#include "domain/backtest/Portfolio.hpp"
#include <memory>
#include <string>
#include <vector>

namespace qga::persistence {

    /**
     * @class IDataStore
     * @brief Abstract interface for data storage operations.
     *
     * Defines methods for saving/loading quotes, bar series, portfolios, and trades.
     * Implementations must handle connection management, transactions, and error handling.
     */
    class IDataStore {
    public:
        /// @brief Virtual destructor.
        virtual ~IDataStore() = default;

        /// @brief Save a vector of quotes to the data store.
        /// @param symbol Symbol identifier (e.g. "AAPL").
        /// @param quotes Vector of Quote objects to save.
        virtual void saveQuotes(const std::string& symbol, const std::vector<domain::Quote>& quotes) = 0;

        /// @brief Load quotes for a given symbol from the data store.
        /// @param symbol Symbol identifier (e.g. "AAPL").
        /// @return Vector of Quote objects loaded from the store.
        virtual std::vector<domain::Quote> loadQuotes(const std::string& symbol) = 0;

        /// @brief Save a BarSeries to the data store.
        /// @param series BarSeries object to save.
        virtual void saveBarSeries(const qga::domain::backtest::BarSeries& series) = 0;

        /// @brief Load a BarSeries for a given symbol from the data store.
        /// @param symbol Symbol identifier (e.g. "AAPL").
        /// @return BarSeries object loaded from the store.
        virtual qga::domain::backtest::BarSeries loadBarSeries(const std::string& symbol) = 0;

        /// @brief Save a Portfolio to the data store.
        /// @param portfolio Portfolio object to save.
        virtual void savePortfolio(const qga::domain::backtest::Portfolio& portfolio) = 0;

        /// @brief Load a Portfolio by its ID from the data store.
        /// @param portfolioId Unique identifier of the portfolio.
        /// @return Portfolio object loaded from the store.
        virtual qga::domain::backtest::Portfolio loadPortfolio(int portfolio_id) = 0;

        // Additional methods for trades, transactions, etc. can be added here
    };

    /// @brief Factory function to create an IDataStore instance based on configuration.
    /// @param config Configuration string (e.g. connection string or file path).
    /// @return Unique pointer to an IDataStore implementation.
    std::unique_ptr<IDataStore> createDataStore(const std::string& config);

} // namespace qga::persistence
