/**
 * @file Config.hpp
 * @brief Global singleton-based application configuration with integrated Logging.
 *
 * Supports:
 * - Loading parameters from JSON and environment variables (QGA_*).
 * - Profile-based configuration (dev/test/prod) via QGA_PROFILE.
 * - Validation and normalization (e.g. thread clamping).
 * - Integrated Logger (created via LoggerFactory).
 * - Input/output paths for CLI / data ingest / exporters.
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
        // === Singleton ===
        static Config& getInstance() noexcept;

        // === Loading ===
        void loadFromFile(const std::filesystem::path& path,
                          std::vector<std::string>* warnings = nullptr);

        void loadFromEnv(std::vector<std::string>* warnings = nullptr);

        void loadDefaults();
        void validate(std::vector<std::string>* warnings = nullptr);

        // === Read-only API ===
        const std::string& profile() const noexcept { return profile_; }

        const std::filesystem::path& dataDir() const noexcept { return data_dir_; }
        int threads() const noexcept { return threads_; }
        LogLevel logLevel() const noexcept { return log_level_; }
        const std::filesystem::path& logFile() const noexcept { return log_file_; }

        // === NEW: Input/Output paths for CLI ===
        const std::filesystem::path& inputPath() const noexcept { return input_path_; }
        const std::filesystem::path& outputPath() const noexcept { return output_path_; }

        void setInputPath(const std::filesystem::path& p) { input_path_ = p; }
        void setOutputPath(const std::filesystem::path& p) { output_path_ = p; }

        // === Rule of Five (disabled) ===
        ~Config() = default;
        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&) = delete;

      private:
        Config() = default;

        // Helpers
        static std::string toLower(std::string s) noexcept;
        static void addWarn(std::vector<std::string>* w, std::string msg);

        void initLogger();

      private:
        // === Profiles ===
        std::string profile_ = "dev";

        // === Core configuration ===
        std::filesystem::path data_dir_ = "data";
        int threads_ = 4;

        LogLevel log_level_ = LogLevel::Info;
        std::filesystem::path log_file_ = "app.log";

        // === NEW: CLI / ingest / exporter paths ===
        std::filesystem::path input_path_ = "";  ///< Source CSV or HTTP URL
        std::filesystem::path output_path_ = ""; ///< Destination CSV/JSON path

        // Integrated logger
        std::shared_ptr<utils::ILogger> logger_;
    };

} // namespace qga::core
