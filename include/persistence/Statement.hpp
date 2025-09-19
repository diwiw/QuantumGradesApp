/**
 * @file Statement.hpp
 * @brief RAII wrapper for SQLite prepared statements.
 *
 * Encapsulates sqlite3_stmt lifecycle (prepare, bind, step, finalize).
 * Used internally by SQLiteStore to simplify database operations.
 */

#pragma once
#include <sqlite3.h>
#include <string>
#include <cstdint>

namespace qga::persistence {

    /**
     * @class Statement
     * @brief RAII wrapper for sqlite3 prepared statements.
     *
     * Provides convenient methods for parameter binding,
     * result iteration, and resource management.
     */
    class Statement {
    public:
        /// @brief Construct a prepared statement from SQL text.
        /// @param db SQLite database handle.
        /// @param sql SQL query with ? parameters.
        Statement(sqlite3* db, const char* sql);

        /// @brief Destructor finalizes the statement.
        ~Statement();

        // Non-copyable
        Statement(const Statement&) = delete;
        Statement& operator=(const Statement&) = delete;

        // Movable
        Statement(Statement&& other) noexcept;
        Statement& operator=(Statement&& other) noexcept;

        // ==== Bind helpers ====
        void bindInt(int index, int value);
        void bindInt64(int index, int64_t value);
        void bindDouble(int index, double value);
        void bindText(int index, const std::string& value);
        void bindNull(int index);

        // ==== Stepping ====
        /// @brief Step through SELECT results row by row.
        /// @return true if a row is available, false if done.
        bool stepRow();

        /// @brief Execute non-SELECT statement (INSERT/UPDATE/DELETE).
        /// @return true if execution finished successfully.
        bool stepDone();

        // ==== Column getters ====
        int getColumnInt(int col) const;
        sqlite3_int64 getColumnInt64(int col) const;
        double getColumnDouble(int col) const;
        const char* getColumnText(int col) const;

        /// @brief Reset statement so it can be re-executed.
        void reset();

        /// @brief Execute a raw DDL (CREATE, DROP, ALTER...).
        static void execDdl(sqlite3* database, const char* sql);

    private:
        sqlite3* database_{};
        sqlite3_stmt* statement_{};
    };

} // namespace qga::persistence
