#include "utils/SpdLogger.hpp"

namespace qga::utils {
    
    SpdLogger::SpdLogger(const std::string& logger_name,
                         const std::string& file_path,
                         LogLevel level)
    {
        // Initialize async logging (queue size = 8192, 1 worker thread)
        spdlog::init_thread_pool(8192, 1);

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_path, true);
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        std::vector<spdlog::sink_ptr> sinks { file_sink, console_sink };
        spd_logger_ = std::make_shared<spdlog::async_logger>(
            logger_name,
            sinks.begin(), 
            sinks.end(),
            spdlog::thread_pool(), 
            spdlog::async_overflow_policy::block);

        spdlog::register_logger(spd_logger_);
        spd_logger_->set_level(toSpdLevel(level));
        spd_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
    }

    SpdLogger::SpdLogger(const std::string& logger_name,
                     const std::vector<std::shared_ptr<spdlog::sinks::sink>>& sinks,
                     bool async_mode) 
    {
        if (async_mode) {
            spd_logger_ = std::make_shared<spdlog::async_logger>(
                logger_name,
                begin(sinks),
                end(sinks),
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block);
        } else {
            spd_logger_ = std::make_shared<spdlog::logger>(logger_name, sinks.begin(), sinks.end());
        }

        spd_logger_->set_level(spdlog::level::trace); // default
        spd_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
    }

    void SpdLogger::log(LogLevel level, const std::string& message)
    {
        if (!spd_logger_) return;

        switch(level) {
            case LogLevel::Trace: return spd_logger_->trace(message); break;
            case LogLevel::Debug: return spd_logger_->debug(message); break;
            case LogLevel::Info: return spd_logger_->info(message); break;
            case LogLevel::Warn: return spd_logger_->warn(message); break;
            case LogLevel::Error: return spd_logger_->error(message); break;
            case LogLevel::Critical: return spd_logger_->critical(message); break;
        }        
    }

    spdlog::level::level_enum SpdLogger::setLevel(LogLevel level)  {
        if (spd_logger_) {
            spd_logger_->set_level(toSpdLevel(level));
            return spd_logger_->level();
        }
        return spdlog::level::info; // default
    }

    spdlog::level::level_enum SpdLogger::toSpdLevel(LogLevel level)
    {
        switch(level) {
            case LogLevel::Trace: return spdlog::level::trace; break;
            case LogLevel::Debug: return spdlog::level::debug; break;
            case LogLevel::Info: return spdlog::level::info; break;
            case LogLevel::Warn: return spdlog::level::warn; break;
            case LogLevel::Error: return spdlog::level::err; break;
            case LogLevel::Critical: return spdlog::level::critical; break;
            default: return spdlog::level::info; break;
        }
        
    }
}   // namespace utils