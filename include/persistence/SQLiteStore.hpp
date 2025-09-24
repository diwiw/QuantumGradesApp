/**
 * @file SQLiteStore.hpp
 * @brief SQLite implementation of IDataStore interface.
 * 
 * Provides persistence using a local SQLite database file.
 * Manages connection lifecycle and executes SQL queries for quotes, bar series, portfolios.
 */

#pragma once

#include "persistence/IDataStore.hpp"
#include "utils/ILogger.hpp"
#include <sqlite3.h>
#include <string>
#include <memory>

struct sqlite3; // forward declaration

namespace qga::persistence {

    /**
     * @class SQLiteStore
     * @brief SQLite-based implementation of IDataStore.
     * 
     * Uses SQLite C API to store and retrieve financial data.
     * Ensures thread safety and efficient connection management.
     */
    class SQLiteStore : public IDataStore {
    public:
        /// @brief Constructor opens (or creates) the SQLite database file.
        /// @param db_path Filesystem path to the SQLite database file.
        explicit SQLiteStore(const std::string& db_path,
                                std::shared_ptr<utils::ILogger> logger = nullptr); //DI

        /// @brief Destructor closes the database connection.
        ~SQLiteStore() override;

        // --- IDataStore interface implementation ---

        void saveQuotes(const std::string& symbol, const std::vector<domain::Quote>& quotes) override;
        std::vector<domain::Quote> loadQuotes(const std::string& symbol) override;
        void saveBarSeries(const qga::domain::backtest::BarSeries& series) override;
        qga::domain::backtest::BarSeries loadBarSeries(const std::string& symbol) override;
        void savePortfolio(const qga::domain::backtest::Portfolio& portfolio) override;
        qga::domain::backtest::Portfolio loadPortfolio(int portfolio_id) override;

    private:
        sqlite3* db_ = nullptr; ///< raw pointer managed internally (RAII in ctor/dtor)
        std::string db_path_; ///< Path to SQLite database file
        std::shared_ptr<utils::ILogger> logger_; ///< Optional logger for diagnostics, DI

        void initSchema(); ///< Create tables if they do not exist
        void open(const std::string& db_file); ///< Open DB connection
        void close(); ///< Close DB connection
    };

} // namespace qga::persistence