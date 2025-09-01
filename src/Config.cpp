#include "Config.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace qga {

    // ===== Utilities =====
    std::string Config::toLower(std::string s) noexcept {
    std::transform(s.begin(), s.end(), s.begin(),
                    [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
    }

    void Config::addWarn(std::vector<std::string>* w, std::string msg) {
    if (w) w->push_back(std::move(msg));
    }

    const char* Config::toString(LogLevel lvl) noexcept {
    switch (lvl) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warn:     return "WARN";
        case LogLevel::Err:      return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        case LogLevel::Off:      return "OFF";
    }
    return "INFO";
    }

    std::optional<LogLevel> Config::parseLogLevel(std::string s) noexcept {
    s = toLower(std::move(s));
    if (s == "trace")    return LogLevel::Trace;
    if (s == "debug")    return LogLevel::Debug;
    if (s == "info")     return LogLevel::Info;
    if (s == "warn" || s == "warning") return LogLevel::Warn;
    if (s == "err"  || s == "error")   return LogLevel::Err;
    if (s == "critical" || s == "crit")return LogLevel::Critical;
    if (s == "off" || s == "none")     return LogLevel::Off;
    return std::nullopt;
    }

    // ===== Singleton =====
    Config& Config::getInstance() noexcept {
    static Config instance; // thread-safe since C++11
    return instance;
    }

    // ===== Public API =====
    void Config::loadDefaults() {
    data_dir_  = "data";
    threads_  = 4;
    log_level_ = LogLevel::Info;
    log_file_  = "app.log";
    validate(nullptr);
    }

    void Config::validate(std::vector<std::string>* warnings) {
    const unsigned HW = std::max(1u, std::thread::hardware_concurrency());

    if (threads_ < 1) {
        addWarn(warnings, "engine.threads < 1 → set to 1");
        threads_ = 1;
    } else if (threads_ > static_cast<int>(HW)) {
        addWarn(warnings, "engine.threads > CPU cores → set to " + std::to_string(HW));
        threads_ = static_cast<int>(HW);
    }

    if (data_dir_.empty()) {
        addWarn(warnings, "paths.data_dir is empty → 'data'");
        data_dir_ = "data";
    }

    if (log_file_.empty()) {
        addWarn(warnings, "logging.file is empty → 'app.log'");
        log_file_ = "app.log";
    }
    }

    void Config::loadFromFile(const std::filesystem::path& path,
                            std::vector<std::string>* warnings) {
    // Start from current values (or defaults if you prefer)
    // Try to open file
    std::ifstream in(path);
    if (!in) {
        addWarn(warnings, "Config file not found: " + path.string() + " (using current/default values).");
        validate(warnings);
        return;
    }

    // Read & parse
    std::string content((std::istreambuf_iterator<char>(in)), {});
    json j = json::parse(content, /*cb*/nullptr, /*allow_exceptions*/false);
    if (j.is_discarded()) {
        addWarn(warnings, "Invalid JSON in " + path.string() + " (using current/default values).");
        validate(warnings);
        return;
    }

    // paths.data_dir
    if (j.contains("paths") && j["paths"].is_object()) {
        const auto& jp = j["paths"];
        if (jp.contains("data_dir")) {
        if (jp["data_dir"].is_string()) data_dir_ = jp["data_dir"].get<std::string>();
        else addWarn(warnings, "paths.data_dir: expected string");
        }
    }

    // engine.threads
    if (j.contains("engine") && j["engine"].is_object()) {
        const auto& je = j["engine"];
        if (je.contains("threads")) {
        if (je["threads"].is_number_integer()) threads_ = je["threads"].get<int>();
        else addWarn(warnings, "engine.threads: expected integer");
        }
    }

    // logging.level / logging.file
    if (j.contains("logging") && j["logging"].is_object()) {
        const auto& jl = j["logging"];
        if (jl.contains("level")) {
        if (jl["level"].is_string()) {
            auto lvl = parseLogLevel(jl["level"].get<std::string>());
            if (lvl) log_level_ = *lvl;
            else addWarn(warnings, "logging.level: unknown value (allowed: trace/debug/info/warn/error/critical/off)");
        } else {
            addWarn(warnings, "logging.level: expected string");
        }
        }
        if (jl.contains("file")) {
        if (jl["file"].is_string()) log_file_ = jl["file"].get<std::string>();
        else addWarn(warnings, "logging.file: expected string");
        }
    }

    validate(warnings);
    }

    void Config::loadFromEnv(std::vector<std::string>* warnings) {
    if (const char* v = std::getenv("QGA_DATA_DIR")) {
        if (*v) data_dir_ = v; else addWarn(warnings, "QGA_DATA_DIR is empty");
    }
    if (const char* v = std::getenv("QGA_THREADS")) {
        try {
        threads_ = std::stoi(v);
        } catch (...) {
        addWarn(warnings, "QGA_THREADS: invalid integer");
        }
    }
    if (const char* v = std::getenv("QGA_LOG_FILE")) {
        if (*v) log_file_ = v; else addWarn(warnings, "QGA_LOG_FILE is empty");
    }
    if (const char* v = std::getenv("QGA_LOG_LEVEL")) {
        if (auto lvl = parseLogLevel(v)) log_level_ = *lvl;
        else addWarn(warnings, "QGA_LOG_LEVEL: unknown value");
    }

    validate(warnings);
    }

} // namespace qga
