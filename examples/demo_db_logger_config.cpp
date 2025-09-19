#include "Config.hpp"
#include "utils/SpdLogger.hpp"
#include "persistence/DatabaseWorker.hpp"

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace qga;
using namespace utils;

#include "Version.hpp"
#include "Config.hpp"

int main() {
	// === Header ===
	std::cout << "===================================\n";
	std::cout << " QuantumGradesApp Backtest\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";
/*
 	// ===== 1. Load Config =====
    auto& config = Config::getInstance();
    config.loadFromEnv();  // np. zmienna QGA_DB_PATH
    std::string db_path = config.dataDir("db.path").value_or("demo.db");
    auto log_level = qga::Config.getInstance().logLevel(); 

    // ===== 2. Setup Logger =====
    auto logger = std::make_shared<utils::SpdLogger>(
        "DemoLogger",
        std::vector<std::shared_ptr<spdlog::sinks::sink>>{
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
        },
        true // async
    );
    logger->setLevel(log_level);
    logger->setLevel("Logger initialized with level {}", static_cast<int>(log_level));

    // ===== 3. Start DatabaseWorker =====
    persistence::DatabaseWorker dbw(db_path);
    logger->info("DatabaseWorker started with DB: {}", db_path);

    // ===== 4. Enqueue schema creation =====
    dbw.enqueue([logger](sqlite3* db) {
        const char* sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
        char* err = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
            logger->error("SQL error: {}", err);
            sqlite3_free(err);
        } else {
            logger->info("Table 'test' ensured.");
        }
    });

    // ===== 5. Enqueue data insert =====
    dbw.enqueue([logger](sqlite3* db) {
        const char* sql = "INSERT INTO test (name) VALUES ('Alice');";
        char* err = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
            logger->error("Insert error: {}", err);
            sqlite3_free(err);
        } else {
            logger->info("Inserted row into 'test'.");
        }
    });

    // ===== 6. Enqueue data select =====
    dbw.enqueue([logger](sqlite3* db) {
        const char* sql = "SELECT id, name FROM test;";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            logger->error("Prepare failed: {}", sqlite3_errmsg(db));
            return;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            logger->info("Row: id={}, name={}", id, name ? name : "NULL");
        }
        sqlite3_finalize(stmt);
    });

    // ===== 7. Allow worker to process tasks =====
    std::this_thread::sleep_for(std::chrono::seconds(2));
    logger->info("Demo finished.");
*/
    return 0;
}