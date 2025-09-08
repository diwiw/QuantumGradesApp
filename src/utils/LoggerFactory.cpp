#include "utils/LoggerFactory.hpp"
#include "utils/SpdLogger.hpp"


namespace utils {


    std::shared_ptr<ILogger> LoggerFactory::createAsyncRotatingLogger(
        const std::string& name,
        const std::string& filename,
        LogLevel level,
        size_t max_size,
        size_t max_files
        ) {
        auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, max_size, max_files);
        auto logger = std::make_shared<SpdLogger>(name, std::vector<spdlog::sink_ptr>{ sink }, true);
        logger->setLevel(level);
        return logger;
    }


    std::shared_ptr<ILogger> LoggerFactory::createConsoleLogger(
        const std::string& name,
        LogLevel level
        ) {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto logger = std::make_shared<SpdLogger>(name, std::vector<spdlog::sink_ptr>{ sink });
        logger->setLevel(level);
        return logger;
    }


    std::shared_ptr<ILogger> LoggerFactory::createNullLogger() {
        auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>();
        return std::make_shared<SpdLogger>("null", std::vector<spdlog::sink_ptr>{ null_sink });
    }

} // namespace utils