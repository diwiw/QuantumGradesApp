/**
 * @file Config.hpp
 * @brief Global singleton-based application configuration with integrated Logging.
 *
 * Supports:
 * - Loading parameters from JSON and environment variables (QGA_*).
 * - Validation and normalization (e.g. thread clamping).
 * - Integrated Logger (created via LoggerFactory).
 */

#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/LogLevel.hpp"
#include "utils/ILogger.hpp"

namespace qga {
  
  /**
   * @class Config
   * @brief Global singleton managing application-wide configuration.
   *
   * Features:
   * - Loaded once at startup from JSON + environment (`QGA_*`).
   * - Normalized (e.g. `threads` clamped to available cores).
   * - Read-only API via getters.
   * - Supports warning collection for user feedback.
   * - Integrated Logger for application-wide use.
   *
   * Usage:
   * ```cpp
   * auto& cfg = Config::getInstance();
   * cfg.loadFromFile("config.json");
   * cfg.loadFromEnv();
   * int threads = cfg.threads();
   * ```
   */
  class Config {
  public:
    // === Singleton access ===

    /**
     * @brief Access the global singleton instance.
     * @return Reference to the singleton.
     */
    static Config& getInstance() noexcept;

    // === Loading / setup ===

    /**
     * @brief Load configuration from a JSON file.
     * @param path Path to JSON file.
     * @param warnings Optional vector to collect warnings (e.g. type mismatches).
     *
     * @note If the file is missing or invalid, the config remains unchanged.
     */
    void loadFromFile(const std::filesystem::path& path,
                      std::vector<std::string>* warnings = nullptr);

    /**
     * @brief Load overrides from environment variables prefixed with `QGA_`.
     * @param warnings Optional vector to collect warnings (e.g. unknown log level).
     */
    void loadFromEnv(std::vector<std::string>* warnings = nullptr);

    /**
     * @brief Reset config to default values and validate.
     *
     * Useful for unit tests or full reload.
     */
    void loadDefaults();

    /**
     * @brief Validate current state and normalize values.
     * @param warnings Optional vector to collect validation warnings.
     *
     * @details Clamps thread count, checks file paths, parses log level.
     */
    void validate(std::vector<std::string>* warnings = nullptr);

    // === Read-only API ===

    /// @return Path to the data directory.
    const std::filesystem::path& dataDir()  const noexcept { return data_dir_; }
    
    /// @return Number of worker threads.
    int                          threads()  const noexcept { return threads_; }
    
    /// @return Current log level.
    LogLevel                     logLevel() const noexcept { return log_level_; }
    
    /// @return Path to the log output file.
    const std::filesystem::path& logFile()  const noexcept { return log_file_; }

    // --- Rule of Five (singleton: non-copyable, non-movable) ---
    ~Config() = default;
    Config(const Config&)            = delete;
    Config(Config&&)                 = delete;
    Config& operator=(const Config&) = delete;
    Config& operator=(Config&&)      = delete;

  private:
    /// Private constructor (singleton pattern).
    Config() = default;

    // === State ===
    std::filesystem::path data_dir_  = "data";   ///< Data directory for input/output files.
    int                   threads_  = 4;         ///< Number of worker threads.
    LogLevel              log_level_ = LogLevel::Info;  ///< Logging verbosity level.
    std::filesystem::path log_file_  = "app.log"; ///< Output file for logs.

    std::shared_ptr<utils::ILogger> logger_; ///< Integrated logger instance.

     // === Internal helpers ===

    /**
     * @brief Convert string to lowercase (for log level parsing).
     * @param s Input string.
     * @return Lowercased string.
     */
    static std::string toLower(std::string s) noexcept;

    /**
     * @brief Add a warning message to the warning vector, if not null.
     * @param w Pointer to vector; may be null.
     * @param msg Warning message to append.
     */
    static void addWarn(std::vector<std::string>* w, std::string msg);

    /**
     * @brief Initialize the integrated logger based on current config.
     *
     * Creates a logger instance (e.g. via LoggerFactory) with the configured
     * log level and output file.
     */
    void initLogger();
  };

} // namespace qga
