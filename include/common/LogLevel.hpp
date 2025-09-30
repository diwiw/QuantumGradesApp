#pragma once
#include <string>
#include <optional>
#include <ostream>

namespace qga {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Err,
    Critical,
    Off
};

// ===== Helpers =====
inline const char* toString(LogLevel lvl) noexcept {
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

inline std::optional<LogLevel> parseLogLevel(std::string s) noexcept {
    // normalizacja
    for (auto& c : s) c = static_cast<char>(std::tolower(c));

    if (s == "trace")    return qga::LogLevel::Trace;
    if (s == "debug")    return qga::LogLevel::Debug;
    if (s == "info")     return qga::LogLevel::Info;
    if (s == "warn" || s == "warning") return qga::LogLevel::Warn;
    if (s == "err"  || s == "error")   return qga::LogLevel::Err;
    if (s == "critical" || s == "crit")return qga::LogLevel::Critical;
    if (s == "off" || s == "none")     return qga::LogLevel::Off;
    return std::nullopt;
}

inline std::ostream& operator<<(std::ostream& os, LogLevel lvl) {
    return os << toString(lvl);
}
} // namespace qga
