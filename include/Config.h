#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace qga {

  enum class LogLevel { trace, debug, info, warn, err, critical, off };

  /// Global application configuration (singleton).
  /// - Load once at startup (from JSON + ENV), then read-only via getters.
  /// - Always validated & normalized (e.g., threads clamped to CPU cores).
  class Config {
  public:
    // === Singleton access ===
    static Config& getInstance() noexcept;

    // === Loading / setup ===
    // Load from JSON file (missing file -> keep current values / defaults).
    // Collects warnings (type mismatches, unknown log level, etc.).
    void loadFromFile(const std::filesystem::path& path,
                      std::vector<std::string>* warnings = nullptr);

    // Apply environment overrides (QGA_*), then validate.
    void loadFromEnv(std::vector<std::string>* warnings = nullptr);

    // Reset to safe defaults (useful for tests), then validate.
    void loadDefaults();

    // Validate & normalize current values. Appends warnings if provided.
    void validate(std::vector<std::string>* warnings = nullptr);

    // === Read-only API ===
    const std::filesystem::path& dataDir()  const noexcept { return dataDir_; }
    int                          threads()  const noexcept { return threads_; }
    LogLevel                     logLevel() const noexcept { return logLevel_; }
    const std::filesystem::path& logFile()  const noexcept { return logFile_; }

    // === Helpers ===
    static const char* toString(LogLevel lvl) noexcept;
    static std::optional<LogLevel> parseLogLevel(std::string s) noexcept;

    // --- Rule of Five (singleton: non-copyable, non-movable) ---
    ~Config() = default;
    Config(const Config&)            = delete;
    Config(Config&&)                 = delete;
    Config& operator=(const Config&) = delete;
    Config& operator=(Config&&)      = delete;

  private:
    // Private ctor: only accessible via getInstance()
    Config() = default;

    // State (always accessed via getters)
    std::filesystem::path dataDir_  = "data";
    int                   threads_  = 4;
    LogLevel              logLevel_ = LogLevel::info;
    std::filesystem::path logFile_  = "app.log";

    // Internal helpers
    static std::string toLower(std::string s) noexcept;
    static void addWarn(std::vector<std::string>* w, std::string msg);
  };

} // namespace qga
