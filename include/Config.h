/**
 * @file Config.h
 * @brief Global singleton-based application configuration.
 *
 * Supports loading parameters from JSON and environment variables (QGA_*),
 * with validation and normalization (e.g. thread clamping).
 */

#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace qga {

  /**
   * @enum LogLevel
   * @brief Logging verbosity levels, similar to spdlog/severity levels.
   */
  enum class LogLevel { 
    trace,     ///< Most detailed logs (development/debug).
    debug,     ///< Debugging information.
    info,      ///< General information (default).
    warn,      ///< Warnings about potential issues.
    err,       ///< Errors that allow app to continue.
    critical,  ///< Critical errors requiring app termination.
    off        ///< Disable logging entirely.
};

  /**
   * @class Config
   * @brief Global singleton managing application-wide configuration.
   *
   * Features:
   * - Loaded once at startup from JSON + environment (`QGA_*`).
   * - Normalized (e.g. `threads` clamped to available cores).
   * - Read-only API via getters.
   * - Supports warning collection for user feedback.
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
    const std::filesystem::path& dataDir()  const noexcept { return dataDir_; }
    
    /// @return Number of worker threads.
    int                          threads()  const noexcept { return threads_; }
    
    /// @return Current log level.
    LogLevel                     logLevel() const noexcept { return logLevel_; }
    
    /// @return Path to the log output file.
    const std::filesystem::path& logFile()  const noexcept { return logFile_; }

     // === Helpers ===

    /**
     * @brief Convert log level enum to string.
     * @param lvl Log level enum.
     * @return Corresponding string representation.
     */
    static const char* toString(LogLevel lvl) noexcept;

    /**
     * @brief Parse log level from string (case-insensitive).
     * @param s Input string (e.g. "info", "debug").
     * @return Parsed LogLevel if valid; otherwise nullopt.
     */
    static std::optional<LogLevel> parseLogLevel(std::string s) noexcept;

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
    std::filesystem::path dataDir_  = "data";   ///< Data directory for input/output files.
    int                   threads_  = 4;         ///< Number of worker threads.
    LogLevel              logLevel_ = LogLevel::info;  ///< Logging verbosity level.
    std::filesystem::path logFile_  = "app.log"; ///< Output file for logs.

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
  };

} // namespace qga
