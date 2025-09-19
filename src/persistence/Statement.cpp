#include "persistence/Statement.hpp"
#include <stdexcept>

namespace qga::persistence {

    Statement::Statement(sqlite3* db, const char* sql) : database_(db) {
        if (sqlite3_prepare_v2(database_, sql, -1, &statement_, nullptr) != SQLITE_OK) {
            throw std::runtime_error(
                std::string("Failed to prepare statement: ") + sqlite3_errmsg(database_));
        }
    }

    Statement::~Statement() {
        if (statement_) {
            sqlite3_finalize(statement_);
        }
    }

    Statement::Statement(Statement&& other) noexcept
        : database_(other.database_), statement_(other.statement_) {
        other.statement_ = nullptr;
        other.database_ = nullptr;
    }

    Statement& Statement::operator=(Statement&& other) noexcept {
        if (this != &other) {
            if (statement_) {
                sqlite3_finalize(statement_);
            }
            database_ = other.database_;
            statement_ = other.statement_;
            other.statement_ = nullptr;
            other.database_ = nullptr;
        }
        return *this;
    }

    // ==== Bind helpers ====
    void Statement::bindInt(int index, int value) {
        if (sqlite3_bind_int(statement_, index, value) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind int parameter");
        }
    }

    void Statement::bindInt64(int index, int64_t value) {
        if (sqlite3_bind_int64(statement_, index, value) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind int64 parameter");
        }
    }

    void Statement::bindDouble(int index, double value) {
        if (sqlite3_bind_double(statement_, index, value) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind double parameter");
        }
    }

    void Statement::bindText(int index, const std::string& value) {
        if (sqlite3_bind_text(statement_, index, value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind text parameter");
        }
    }

    void Statement::bindNull(int index) {
        if (sqlite3_bind_null(statement_, index) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind null parameter");
        }
    }

    // ==== Stepping ====
    bool Statement::stepRow() {
        int rc = sqlite3_step(statement_);
        if (rc == SQLITE_ROW) {
            return true;
        } else if (rc == SQLITE_DONE) {
            return false;
        }
        throw std::runtime_error("Failed to step statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    bool Statement::stepDone() {
        int rc = sqlite3_step(statement_);
        if (rc == SQLITE_DONE) {
            return true;
        } else if (rc == SQLITE_ROW) {
            return false;
        }
        throw std::runtime_error("Failed to execute statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    // ==== Column getters ====
    int Statement::getColumnInt(int col) const {
        return sqlite3_column_int(statement_, col);
    }

    sqlite3_int64 Statement::getColumnInt64(int col) const {
        return sqlite3_column_int64(statement_, col);
    }

    double Statement::getColumnDouble(int col) const {
        return sqlite3_column_double(statement_, col);
    }

    const char* Statement::getColumnText(int col) const {
        auto p = reinterpret_cast<const char*>(sqlite3_column_text(statement_, col));
        return p ? p : "";
    }

    void Statement::reset() {
        if (sqlite3_reset(statement_) != SQLITE_OK) {
            throw std::runtime_error("Failed to reset statement");
        }
        if (sqlite3_clear_bindings(statement_) != SQLITE_OK) {
            throw std::runtime_error("Failed to clear statement bindings");
        }
    }

    void Statement::execDdl(sqlite3* database, const char* sql) {
        char* err_msg = nullptr;
        if (sqlite3_exec(database, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::string error = err_msg ? err_msg : "Unknown error";
            sqlite3_free(err_msg);
            throw std::runtime_error("DDL failed: " + error);
        }
    }

} // namespace qga::persistence
