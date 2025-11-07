#include "persistence/SQLiteStore.hpp"
#include "persistence/Statement.hpp"
#include "utils/ILogger.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace qga::persistence {

    SQLiteStore::SQLiteStore(const std::string& db_file,
                             std::shared_ptr<utils::ILogger> logger)
        : db_path_(db_file), logger_(std::move(logger)) {
        if (sqlite3_open_v2(db_path_.c_str(), &db_,
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
                            nullptr) != SQLITE_OK) {
            db_ = nullptr;
            throw std::runtime_error("Could not open SQLite database: " +
                                     std::string(sqlite3_errmsg(db_)));
        }
        if (logger_) {
            logger_->info("Opened SQLite database at: " + db_path_);
        }

        // Minimal schema
        Statement::execDdl(db_,
            "PRAGMA foreign_keys=ON;"
            "CREATE TABLE IF NOT EXISTS quotes("
            "  symbol TEXT NOT NULL,"
            "  ts     INTEGER NOT NULL,"    // epoch ms
            "  open   REAL NOT NULL,"
            "  high   REAL NOT NULL,"
            "  low    REAL NOT NULL,"
            "  close  REAL NOT NULL,"
            "  volume REAL NOT NULL,"
            "  PRIMARY KEY(symbol, ts)"
            ");"
        );
    }

    SQLiteStore::~SQLiteStore() {
        if (db_) {
            if (logger_) {
                logger_->info("Closed SQLite database at: " + db_path_);
            }
            sqlite3_close(db_);
            db_ = nullptr;
        }
    }

    void SQLiteStore::saveQuotes(const std::string& symbol,
                                 const std::vector<domain::Quote>& quotes) {
        if (!db_) throw std::runtime_error("Database not open");
        if (quotes.empty()) return;

        Statement::execDdl(db_, "BEGIN IMMEDIATE;");
        try {
            Statement ins{db_,
                "INSERT INTO quotes (symbol, ts, open, high, low, close, volume) "
                "VALUES (?, ?, ?, ?, ?, ?, ?) "
                "ON CONFLICT(symbol, ts) DO UPDATE SET "
                "open = excluded.open, high = excluded.high, "
                "low = excluded.low, close = excluded.close, volume = excluded.volume;"
            };

            for (const auto& q : quotes) {
                ins.bindText(1, symbol);
                ins.bindInt64(2, static_cast<sqlite3_int64>(q.ts_));
                ins.bindDouble(3, q.open_);
                ins.bindDouble(4, q.high_);
                ins.bindDouble(5, q.low_);
                ins.bindDouble(6, q.close_);
                ins.bindDouble(7, q.volume_);

                if (!ins.stepDone()) {
                    throw std::runtime_error("Failed to insert/update quote");
                }
                ins.reset();
            }

            Statement::execDdl(db_, "COMMIT;");
            if (logger_) {
                logger_->info("Saved " + std::to_string(quotes.size()) +
                              " quotes for symbol: " + symbol);
            }
        } catch (...) {
            Statement::execDdl(db_, "ROLLBACK;");
            if (logger_) {
                logger_->error("saveQuotes rollback for " + symbol);
            }
            throw;
        }
    }

    std::vector<domain::Quote> SQLiteStore::loadQuotes(const std::string& symbol) {
        if (!db_) throw std::runtime_error("Database not open");

        std::vector<domain::Quote> quotes;
        Statement sel{db_, "SELECT ts, open, high, low, close, volume "
                           "FROM quotes WHERE symbol = ? ORDER BY ts ASC;"};
        sel.bindText(1, symbol);

        while (sel.stepRow()) {
            domain::Quote q{};
            q.ts_     = static_cast<std::int64_t>(sel.getColumnInt64(0));
            q.open_   = sel.getColumnDouble(1);
            q.high_   = sel.getColumnDouble(2);
            q.low_    = sel.getColumnDouble(3);
            q.close_  = sel.getColumnDouble(4);
            q.volume_ = sel.getColumnDouble(5);
            quotes.push_back(q);
        }

        if (logger_) {
            logger_->info("Loaded " + std::to_string(quotes.size()) +
                          " quotes for symbol: " + symbol);
        }
        return quotes;
    }

    void SQLiteStore::saveBarSeries(const qga::domain::backtest::BarSeries& /*series*/) {
        // TODO: implement with CREATE TABLE bars(...) + INSERT loop
        throw std::runtime_error("saveBarSeries not implemented yet");
    }

    qga::domain::backtest::BarSeries SQLiteStore::loadBarSeries(const std::string& /*symbol*/) {
        throw std::runtime_error("loadBarSeries not implemented yet");
    }

    void SQLiteStore::savePortfolio(const qga::domain::backtest::Portfolio& /*portfolio*/) {
        throw std::runtime_error("savePortfolio not implemented yet");
    }

    qga::domain::backtest::Portfolio SQLiteStore::loadPortfolio(int /*portfolio_id*/) {
        throw std::runtime_error("loadPortfolio not implemented yet");
    }

} // namespace qga::persistence
