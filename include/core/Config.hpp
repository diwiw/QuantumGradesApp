/**
 * @file Config.hpp
 * @brief Global singleton-based application configuration with integrated Logging.
 *
 * Supports:
 * - Loading parameters from JSON and environment variables (QGA_*).
 * - Profile-based configuration (dev/test/prod) via QGA_PROFILE.
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

namespace qga::core
{

    class Config
    {
      public:
        // === Singleton access ===
        static Config& getInstance() noexcept;

        // === Loading / setup ===

        /**
         * @brief Load configuration from a JSON file.
         * @param path Path to JSON file.
         * @param warnings Optional vector to collect warnings.
         *
         * If the file is missing, config remains unchanged.
         */
        void loadFromFile(const std::filesystem::path& path,
                          std::vector<std::string>* warnings = nullptr);

        /**
         * @brief Load overrides from environment variables prefixed with QGA_.
         *
         * New behavior:
         * - Detects QGA_PROFILE={dev|test|prod}
         * - Loads corresponding config/config.<profile>.json
         */
        void loadFromEnv(std::vector<std::string>* warnings = nullptr);

        /**
         * @brief Reset config to default values and validate.
         */
        void loadDefaults();

        /**
         * @brief Validate current state and normalize values.
         */
        void validate(std::vector<std::string>* warnings = nullptr);

        // === Read-only API ===

        const std::string& profile() const noexcept { return profile_; }

        const std::filesystem::path& dataDir() const noexcept { return data_dir_; }
        int threads() const noexcept { return threads_; }
        LogLevel logLevel() const noexcept { return log_level_; }
        const std::filesystem::path& logFile() const noexcept { return log_file_; }

        // === Rule of Five (disabled for singleton) ===
        ~Config() = default;
        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&) = delete;

      private:
        Config() = default;

        // === Internal helpers ===
        static std::string toLower(std::string s) noexcept;
        static void addWarn(std::vector<std::string>* w, std::string msg);

        /// Initialize integrated logger after config load/validate.
        void initLogger();

      private:
        // === NEW: profile state ===
        std::string profile_ = "dev"; ///< Active configuration profile: dev/test/prod

        // === Existing configuration values ===
        std::filesystem::path data_dir_ = "data";
        int threads_ = 4;
        LogLevel log_level_ = LogLevel::Info;
        std::filesystem::path log_file_ = "app.log";

        std::shared_ptr<utils::ILogger> logger_;
    };

} // namespace qga::core
