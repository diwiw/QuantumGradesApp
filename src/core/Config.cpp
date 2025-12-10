#include "core/Config.hpp"
#include "nlohmann/json.hpp"
#include "utils/LoggerFactory.hpp"
#include "Version.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <thread>

using nlohmann::json;

namespace qga::core {

// ============================================================
// Singleton
// ============================================================
Config& Config::getInstance() noexcept {
    static Config instance;
    return instance;
}

// ============================================================
// Helper utilities
// ============================================================
std::string Config::toLower(std::string s) noexcept {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

void Config::addWarn(std::vector<std::string>* w, std::string msg) {
    if (w)
        w->push_back(std::move(msg));
}

// ============================================================
// Defaults
// ============================================================
void Config::loadDefaults() {
    profile_ = "dev";
    api_port_ = 8080;
    threads_ = 4;
    data_dir_ = "data";

    log_level_ = LogLevel::Info;
    log_file_ = "logs/qga.log";
    log_max_size_mb_ = 10;
    log_max_files_ = 3;

    version_ = APP_VERSION;

    validate(nullptr);
}

// ============================================================
// Validation
// ============================================================
void Config::validate(std::vector<std::string>* warnings) {
    unsigned hw = std::max(1u, std::thread::hardware_concurrency());

    // Threads validation
    if (threads_ < 1) {
        addWarn(warnings, "engine.threads < 1 → fallback to 1");
        threads_ = 1;
    } else if (threads_ > static_cast<int>(hw)) {
        addWarn(warnings, "engine.threads > CPU cores → fallback to hw concurrency");
        threads_ = hw;
    }

    if (log_max_size_mb_ < 1)
        log_max_size_mb_ = 1;

    if (log_max_files_ < 1)
        log_max_files_ = 1;

    initLogger();
}

// ============================================================
// JSON Loader
// ============================================================
void Config::loadFromFile(const std::filesystem::path& path,
                          std::vector<std::string>* warnings) {
    std::ifstream in(path);
    if (!in) {
        addWarn(warnings, "Config file not found: " + path.string());
        loadDefaults();
        return;
    }

    json j = json::parse(in, nullptr, false);
    if (j.is_discarded()) {
        addWarn(warnings, "Invalid JSON in config file: " + path.string());
        loadDefaults();
        return;
    }

    // --------------------------------------------------------
    // API
    // --------------------------------------------------------
    if (j.contains("api") && j["api"].contains("port"))
        api_port_ = j["api"]["port"].get<int>();

    // --------------------------------------------------------
    // paths
    // --------------------------------------------------------
    if (j.contains("paths") && j["paths"].contains("data_dir"))
        data_dir_ = j["paths"]["data_dir"].get<std::string>();

    // --------------------------------------------------------
    // engine
    // --------------------------------------------------------
    if (j.contains("engine") && j["engine"].contains("threads"))
        threads_ = j["engine"]["threads"].get<int>();

    // --------------------------------------------------------
    // logging
    // --------------------------------------------------------
    if (j.contains("logging")) {
        auto& jl = j["logging"];

        if (jl.contains("level"))
            log_level_ = parseLogLevel(jl["level"].get<std::string>()).value_or(LogLevel::Info);

        if (jl.contains("file"))
            log_file_ = jl["file"].get<std::string>();

        if (jl.contains("max_size_mb"))
            log_max_size_mb_ = jl["max_size_mb"].get<int>();

        if (jl.contains("max_files"))
            log_max_files_ = jl["max_files"].get<int>();
    }

    // --------------------------------------------------------
    // ingest/cli
    // --------------------------------------------------------
    if (j.contains("input") && j["input"].contains("path"))
        input_path_ = j["input"]["path"].get<std::string>();

    if (j.contains("output") && j["output"].contains("path"))
        output_path_ = j["output"]["path"].get<std::string>();

    // Version always overridable
    version_ = APP_VERSION;

    validate(warnings);
}

// ============================================================
// ENV loader
// ============================================================
void Config::loadFromEnv(std::vector<std::string>* warnings) {
    // QGA_PROFILE
    if (const char* p = std::getenv("QGA_PROFILE"))
        profile_ = p;

    loadFromFile("config/config." + profile_ + ".json", warnings);

    // API PORT
    if (const char* p = std::getenv("QGA_API_PORT"))
        api_port_ = std::atoi(p);

    validate(warnings);
}

// ============================================================
// Logger initialization
// ============================================================
void Config::initLogger() {
    logger_ = utils::LoggerFactory::createAsyncRotatingLogger(
        "QGA",
        log_file_.string(),
        log_level_,
        log_max_size_mb_ * 1024 * 1024,
        log_max_files_
    );

    if (logger_)
        logger_->info("Config loaded [profile={}, port={}, version={}]", profile_, api_port_, version_);
}

} // namespace qga::core
